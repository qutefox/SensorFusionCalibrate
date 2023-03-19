#include "serial_port.h"

#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

#include "parse_numbers.h"

SerialPortDataSource::SerialPortDataSource(QWidget* parent)
    : IDataSource{ parent }
    , m_timer{ new QTimer(this) }
    , m_readLoopTimer{ new QTimer(this) }
{
    m_portComboBox = new QComboBox();
    m_portComboBox->setMinimumWidth(220);
    m_portComboBox->addItem("", "");

    m_baudComboBox = new QComboBox();
    for (int baud :QSerialPortInfo::standardBaudRates())
    {
        m_baudComboBox->addItem(QString::number(baud), baud);
    }
    int baudIndex = m_baudComboBox->findData(115200);
    if (baudIndex != -1) m_baudComboBox->setCurrentIndex(baudIndex);

    m_toolButtonConfig =  new QToolButton();
    m_toolButtonConfig->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolButtonConfig->setIcon(QIcon(":/images/config-black.png"));
    m_toolButtonConfig->setText("Config");

    m_toolButtonProcess =  new QToolButton();
    m_toolButtonProcess->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolButtonProcess->setStyleSheet("QToolButton{color:black;}");
    m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
    m_toolButtonProcess->setText("Open");
    m_toolButtonProcess->setEnabled(false);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Port: "));
    layout->addWidget(m_portComboBox);
    layout->addWidget(new QLabel("Baud rate: "));
    layout->addWidget(m_baudComboBox);
    layout->addWidget(m_toolButtonConfig);
    layout->addWidget(m_toolButtonProcess);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    connect(
        m_portComboBox, SIGNAL(currentTextChanged(QString)),
        this, SLOT(serialPortChanged(QString))
    );

    connect(
        &m_serial, SIGNAL(readyRead()),
        this, SLOT(readyRead())
    );

    connect(
        &m_serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
        this, SLOT(onSerialError(QSerialPort::SerialPortError))
    );

    connect(
        m_toolButtonProcess, SIGNAL(released()),
        this, SLOT(processButtonReleased())
    );

    connect(
        m_timer, SIGNAL(timeout()),
        this, SLOT(update())
    );
    m_timer->start(2000);

    m_readLoopTimer->setSingleShot(true);
    connect(
        m_readLoopTimer, SIGNAL(timeout()),
        this, SLOT()
    );
}

SerialPortDataSource::~SerialPortDataSource()
{
    m_timer->stop();
    m_serial.close();
}

void SerialPortDataSource::exitReadLoop()
{
    m_exitReadLoop = true;
}

bool SerialPortDataSource::getNextPoints(std::vector<std::set<Point>>& devicePoints)
{
    devicePoints.clear();
    bool gotData = false;

    m_readLoopTimer->start(250);

    m_exitReadLoop = false;
    while (m_serial.isOpen() && m_serial.canReadLine() && !m_exitReadLoop)
    {
        QByteArray lineByteArray = m_serial.readLine();
        // TODO: future feature possibility to add a serial terminal.
        qDebug() << QString(lineByteArray);
        gotData |= parseLineToDeviceData(lineByteArray.toStdString(), devicePoints);
    }

    return gotData;
}

void SerialPortDataSource::update()
{
    updateSerialList();
}

QString SerialPortDataSource::GenerateReadableSerialPortName(const QSerialPortInfo& serialPort)
{
    QString out = serialPort.portName();
    if(serialPort.hasVendorIdentifier())
    {
        out += QString(", vId: 0x%1").arg(serialPort.vendorIdentifier(), 4, 16, QLatin1Char('0'));
    }
    if(serialPort.hasProductIdentifier())
    {
        out += QString(", pId: 0x%1").arg(serialPort.productIdentifier(), 4, 16, QLatin1Char('0'));
    }
    return out;
}

QString SerialPortDataSource::currentPortComboBoxString() const
{
    return m_portComboBox->itemData(m_portComboBox->currentIndex()).toString();
}

void SerialPortDataSource::updateSerialList()
{
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
    QString prevSerialPort = currentPortComboBoxString();

    QString itemData;
    bool found = false;
    for (int i = 0 ; i < m_portComboBox->count() ; ++i)
    {
        itemData = m_portComboBox->itemData(i).toString();
        if (itemData == "") continue; // Keep empty item.

        found = false;
        for (const QSerialPortInfo& serialPort : serialPorts)
        {
            if (!serialPort.hasVendorIdentifier()) continue;
            if (itemData == serialPort.portName())
            {
                found = true;
                break;
            }
        }

        if (!found) m_portComboBox->removeItem(i);
    }

    for (const QSerialPortInfo& serialPort : serialPorts)
    {
        if (!serialPort.hasVendorIdentifier()) continue;
        if (m_portComboBox->findData(QVariant(serialPort.portName())) != -1) continue;

        QString serialName = GenerateReadableSerialPortName(serialPort);
        m_portComboBox->addItem(serialName, QVariant(serialPort.portName()));
    }

    QString afterSerialPort = currentPortComboBoxString();

    if (prevSerialPort != afterSerialPort)
    {
        int portIndex = m_portComboBox->findData("");
        if (portIndex != -1) m_portComboBox->setCurrentIndex(portIndex);

        serialPortChanged("");
    }

    if (m_serialOpen != m_serial.isOpen())
    {
        serialPortChanged(afterSerialPort);
    }
}

void SerialPortDataSource::serialPortChanged(QString newText)
{
    Q_UNUSED(newText)

    QString selectedSerialPort = currentPortComboBoxString();

    if (!m_serial.isOpen())
    {
        if (selectedSerialPort.isEmpty())
        {
            m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
            m_toolButtonProcess->setStyleSheet("QToolButton{color:black;}");
            m_toolButtonProcess->setText("Open");
            m_toolButtonProcess->setEnabled(false);
        }
        else
        {
            m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
            m_toolButtonProcess->setStyleSheet("QToolButton{color:green;}");
            m_toolButtonProcess->setText("Open");
            m_toolButtonProcess->setEnabled(true);
        }
        return;
    }

    if (selectedSerialPort != m_serial.portName())
    {
        m_serial.close();
        m_serialOpen = false;

        if (selectedSerialPort.isEmpty())
        {
            m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
            m_toolButtonProcess->setStyleSheet("QToolButton{color:black;}");
            m_toolButtonProcess->setText("Open");
            m_toolButtonProcess->setEnabled(false);
        }
        else
        {
            m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
            m_toolButtonProcess->setStyleSheet("QToolButton{color:green;}");
            m_toolButtonProcess->setText("Open");
            m_toolButtonProcess->setEnabled(true);
        }
        return;
    }

    int selectedBaudRate = m_baudComboBox->itemData(m_baudComboBox->currentIndex()).toInt();
    if (selectedBaudRate != m_serial.baudRate())
    {
        m_serial.setBaudRate(selectedBaudRate);
    }
}

void SerialPortDataSource::processButtonReleased()
{
    QString selectedSerialPort = currentPortComboBoxString();

    if (selectedSerialPort.isEmpty())
    {
        m_serial.close();
        m_serialOpen = false;

        m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
        m_toolButtonProcess->setStyleSheet("QToolButton{color:black;}");
        m_toolButtonProcess->setText("Open");
        m_toolButtonProcess->setEnabled(false);
        return;
    }

    if (m_serial.isOpen())
    {
        m_serial.close();
        m_serialOpen = false;

        m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
        m_toolButtonProcess->setStyleSheet("QToolButton{color:green;}");
        m_toolButtonProcess->setText("Open");
        m_toolButtonProcess->setEnabled(true);
    }
    else
    {
        int selectedBaudRate = m_baudComboBox->itemData(m_baudComboBox->currentIndex()).toInt();

        m_serial.setPortName(selectedSerialPort);
        m_serial.setBaudRate(selectedBaudRate);
        m_serial.setDataBits(QSerialPort::DataBits::Data8);
        m_serial.setParity(QSerialPort::Parity::NoParity);
        m_serial.setStopBits(QSerialPort::StopBits::OneStop);
        m_serial.setFlowControl(QSerialPort::FlowControl::NoFlowControl);

        if (m_serial.open(QIODevice::ReadWrite))
        {
            m_serialOpen = true;

            m_toolButtonProcess->setIcon(QIcon(":/images/close-black.png"));
            m_toolButtonProcess->setStyleSheet("QToolButton{color:orange;}");
            m_toolButtonProcess->setText("Close");
            m_toolButtonProcess->setEnabled(true);
        }
        else
        {
            m_serialOpen = false;

            m_toolButtonProcess->setIcon(QIcon(":/images/open-black.png"));
            m_toolButtonProcess->setStyleSheet("QToolButton{color:red;}");
            m_toolButtonProcess->setText("Open");
            m_toolButtonProcess->setEnabled(true);
        }
    }
}

void SerialPortDataSource::readyRead()
{
    emit dataAvailable();
}

void SerialPortDataSource::onSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::SerialPortError::NoError) return;
    emit failed(m_serial.errorString());
}

