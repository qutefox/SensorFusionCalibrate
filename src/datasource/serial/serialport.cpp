#include "serialport.h"

#include <datasource/line_parser.h>

constexpr char ESC = '\x1B';

SerialPortDataSource::SerialPortDataSource(const SerialPortConfig& config, QObject* parent)
    : IDataSource{ parent }
    , m_timer{ new QTimer(this) }
    , m_widget{ new SerialPortControlWidget(config) }
{
    connect(
        m_widget, &SerialPortControlWidget::handlePort,
        this, &SerialPortDataSource::handlePort
    );

    connect(
        m_widget, &SerialPortControlWidget::updatePort,
        this, &SerialPortDataSource::updatePort
    );

    connect(
        &m_serial, &QSerialPort::readyRead,
        this, &SerialPortDataSource::onSerialReadyRead
    );

    connect(
        &m_serial, &QSerialPort::errorOccurred,
        this, &SerialPortDataSource::onSerialError
    );

    connect(
        m_timer, &QTimer::timeout,
        this, &SerialPortDataSource::readLoopTimedOut
    );
}

SerialPortDataSource::~SerialPortDataSource()
{
    m_timer->stop();
    m_serial.close();
    delete m_widget;
}

QWidget* SerialPortDataSource::widget() const
{
    return m_widget;
}

void SerialPortDataSource::readLoopTimedOut()
{
    m_readLoopTimeout = true;
}

bool SerialPortDataSource::getNextPoints(std::vector<std::set<Point>>& devicePoints)
{
    devicePoints.clear();
    bool gotData = false;

    m_timer->start(250);

    m_readLoopTimeout = false;
    while (m_serial.isOpen() && m_serial.canReadLine() && !m_readLoopTimeout)
    {
        QByteArray lineByteArray = m_serial.readLine();
        // TODO: future feature possibility to add a serial terminal.
        qDebug() << QString(lineByteArray);
        gotData |= parseLineToDeviceData(lineByteArray.toStdString(), devicePoints);
    }

    if (!m_serial.isOpen()) m_widget->deviceDisconnected();

    return gotData;
}

void SerialPortDataSource::handlePort(SerialPortConfig config)
{
    if (m_serial.isOpen())
    {
        m_serial.close();
        m_widget->deviceDisconnected();
        return;
    }

    updatePort(config);

    if (config.m_portName.isEmpty()) return;

    if (m_serial.open(QIODevice::ReadWrite)) m_widget->deviceConnected();
    else
    {
        QString errStr = m_serial.errorString();
        if (errStr.isEmpty()) errStr = "Failed to open serial port: " + config.m_portName;
        emit errorOccurred(errStr);
        m_widget->deviceDisconnected();
    }
}

void SerialPortDataSource::updatePort(SerialPortConfig config)
{
    bool openState = m_serial.isOpen();
    if (openState) m_serial.close();
    m_serial.setPortName(config.m_portName);
    m_serial.setBaudRate(config.m_baudRate);
    m_serial.setDataBits(config.m_dataBits);
    m_serial.setParity(config.m_parity);
    m_serial.setStopBits(config.m_stopBits);
    m_serial.setFlowControl(config.m_flowControl);
    if (openState && !config.m_portName.isEmpty())
    {
        if (m_serial.open(QIODevice::ReadWrite)) m_widget->deviceConnected();
        else
        {
            QString errStr = m_serial.errorString();
            if (errStr.isEmpty()) errStr = "Failed to open serial port: " + config.m_portName;
            emit errorOccurred(errStr);
            m_widget->deviceDisconnected();
        }
    }
}

void SerialPortDataSource::onSerialReadyRead()
{
    emit dataAvailable();
}

void SerialPortDataSource::onSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::SerialPortError::NoError) return;
    if (!m_serial.isOpen()) m_widget->deviceDisconnected();
    emit errorOccurred(m_serial.errorString());
}
