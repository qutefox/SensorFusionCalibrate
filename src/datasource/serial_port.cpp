#include "serial_port.h"

#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

SerialPortDataSource::SerialPortDataSource(QWidget* parent)
    : IDataSource{ parent }
    , m_timer{ new QTimer(this) }
{
    m_portComboBox = new QComboBox();
    m_portComboBox->setMinimumWidth(250);
    m_portComboBox->addItem("", "");
    m_baudComboBox = new QComboBox();

    m_toolButtonConfig =  new QToolButton();
    m_toolButtonConfig->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolButtonConfig->setIcon(QIcon(":/images/config-black.png"));
    m_toolButtonConfig->setText("Config");

    m_toolButtonProcess =  new QToolButton();
    m_toolButtonProcess->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
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

    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(1000);
}

SerialPortDataSource::~SerialPortDataSource()
{
    m_timer->stop();
    m_serial.close();
}

bool SerialPortDataSource::getNextPoints(std::vector<std::set<Point>>& devicePoints)
{
    devicePoints.clear();

    if (!m_serial.isOpen()) return false;

    // TODO: read serial data and parse line by line similarly as in csv_file.cpp

    return false;
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

    /*
    if (!m_serial.isOpen())
    {
        if (selectedSerialPort == "")
        {

            // button->setText("Select a port");
            // button->setStyleSheet("QPushButton {color:black;}");
            // button->setDisabled(true);
        }
        else
        {
            // button->setText("Open");
            // button->setStyleSheet("QPushButton {color:green;}");
            // button->setDisabled(false);
        }
        return;
    }
    */
}
