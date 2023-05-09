#include "serialport.h"

#include <datasource/line_parser.h>

SerialPortDataSource::SerialPortDataSource(const SerialPortConfig& config, QObject* parent)
    : IDataSource{ parent }
    , m_readLoopTimeoutTimer{ new QTimer(this) }
    , m_widget{ new SerialPortControlWidget(config) }
    , m_terminal{ new SerialPortTerminalWidget() }
    , m_visualize{ new VisualizeWidget(m_widget) }
    , m_seq{ std::make_shared<vte::Sequencer>(*this) }
	, m_parser{ std::make_unique<vte::parser::Parser>(m_seq) }
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
        m_readLoopTimeoutTimer, &QTimer::timeout,
        this, &SerialPortDataSource::readLoopTimedOut
    );
}

SerialPortDataSource::~SerialPortDataSource()
{
    m_readLoopTimeoutTimer->stop();
    m_serial.close();
    m_parser.reset();
	m_seq.reset();
    delete m_widget;
    delete m_terminal;
}

QWidget* SerialPortDataSource::widget() const
{
    return m_widget;
}

void SerialPortDataSource::makeTabs(QTabWidget* parent) const
{
    parent->insertTab(parent->count()+1, m_terminal, QIcon(""), "Terminal");
    parent->insertTab(parent->count()+1, m_visualize, QIcon(""), "Visual");
}

void SerialPortDataSource::readLoopTimedOut()
{
    m_readLoopTimeout = true;
}

void SerialPortDataSource::executeControlCode(char controlCode)
{
    m_terminal->executeControlCode(controlCode);

    if (controlCode != 0x0A) return;

    m_bufferLock.lockForWrite();
    parseLineToDeviceData(m_lineBuffer.toStdString(), m_devicePointsBuffer);
    m_lineBuffer.clear();
    m_bufferLock.unlock();
}

void SerialPortDataSource::processSequence(vte::Sequence const& sequence)
{
	m_terminal->processSequence(sequence);
}

void SerialPortDataSource::writeText(char32_t codepoint)
{
    m_terminal->writeText(codepoint);

    m_bufferLock.lockForWrite();
    m_lineBuffer += QChar(codepoint);
    m_bufferLock.unlock();
}

void SerialPortDataSource::writeText(std::string_view codepoints, std::size_t cellCount)
{
    m_terminal->writeText(codepoints, cellCount);

    m_bufferLock.lockForWrite();
    m_lineBuffer += QString::fromUtf8(codepoints.data(), cellCount);
    m_bufferLock.unlock();
}

bool SerialPortDataSource::getNextPoints(std::vector<std::set<Point>>& devicePoints)
{
    devicePoints.clear();
    m_readLoopTimeoutTimer->start(250);

    m_readLoopTimeout = false;
    while (m_serial.isOpen() && m_serial.canReadLine() && !m_readLoopTimeout)
    {
        m_parser->parseFragment(m_serial.readLine().toStdString());
    }

    m_bufferLock.lockForRead();
    if (m_devicePointsBuffer.size())
    {
        std::swap(devicePoints, m_devicePointsBuffer);
    }
    m_bufferLock.unlock();

    if (!m_serial.isOpen()) m_widget->deviceDisconnected();

    return devicePoints.size();
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
