#include "serialport_control.h"
#include "./ui_serialport_control.h"

#include <QSerialPortInfo>

#include <datasource/serial/id_matcher.h>

SerialPortControlWidget::SerialPortControlWidget(const SerialPortConfig& config, QWidget *parent)
    : QWidget(parent)
    , m_ui{ new Ui::SerialPortControlWidget }
    , m_updateTimer{ new QTimer(this) }
{
    m_ui->setupUi(this);
    m_ui->autoConnectCheckBox->hide();
    setupDropdownValues();
    setValues(config);
    setupSignalSlot();
}

SerialPortControlWidget::~SerialPortControlWidget()
{
    delete m_ui;
}

void SerialPortControlWidget::setupDropdownValues()
{
    m_ui->serialComboBox->addItem("", "");

    for (int baud : QSerialPortInfo::standardBaudRates())
    {
        m_ui->baudComboBox->addItem(QString::number(baud), baud);
    }

    m_ui->dataBitsComboBox->addItem("5", QSerialPort::DataBits::Data5);
    m_ui->dataBitsComboBox->addItem("6", QSerialPort::DataBits::Data6);
    m_ui->dataBitsComboBox->addItem("7", QSerialPort::DataBits::Data7);
    m_ui->dataBitsComboBox->addItem("8", QSerialPort::DataBits::Data8);

    m_ui->parityComboBox->addItem("None", QSerialPort::Parity::NoParity);
    m_ui->parityComboBox->addItem("Even", QSerialPort::Parity::EvenParity);
    m_ui->parityComboBox->addItem("Odd", QSerialPort::Parity::OddParity);
    m_ui->parityComboBox->addItem("Space", QSerialPort::Parity::SpaceParity);
    m_ui->parityComboBox->addItem("Mark", QSerialPort::Parity::MarkParity);

    m_ui->stopBitsComboBox->addItem("1", QSerialPort::StopBits::OneStop);
    m_ui->stopBitsComboBox->addItem("1.5", QSerialPort::StopBits::OneAndHalfStop);
    m_ui->stopBitsComboBox->addItem("2", QSerialPort::StopBits::TwoStop);

    m_ui->flowControlComboBox->addItem("None", QSerialPort::FlowControl::NoFlowControl);
    m_ui->flowControlComboBox->addItem("Hardware", QSerialPort::FlowControl::HardwareControl);
    m_ui->flowControlComboBox->addItem("Software", QSerialPort::FlowControl::SoftwareControl);
}

void SerialPortControlWidget::setupSignalSlot()
{
    connect(
        m_ui->serialComboBox, &QComboBox::currentIndexChanged,
        this, &SerialPortControlWidget::portChangedSlot
    );
    connect(
        m_ui->baudComboBox, &QComboBox::currentIndexChanged,
        this, &SerialPortControlWidget::baudRateChangedSlot
    );
    connect(
        m_ui->dataBitsComboBox, &QComboBox::currentIndexChanged,
        this, &SerialPortControlWidget::dataBitsChangedSlot
    );
    connect(
        m_ui->parityComboBox, &QComboBox::currentIndexChanged,
        this, &SerialPortControlWidget::parityChangedSlot
    );
    connect(
        m_ui->stopBitsComboBox, &QComboBox::currentIndexChanged,
        this, &SerialPortControlWidget::stopBitsChangedSlot
    );
    connect(
        m_ui->flowControlComboBox, &QComboBox::currentIndexChanged,
        this, &SerialPortControlWidget::flowControlChangedSlot
    );
    connect(
        m_ui->processToolButton, &QToolButton::released,
        this, &SerialPortControlWidget::processReleasedSlot
    );
    connect(
        m_updateTimer, &QTimer::timeout,
        this, &SerialPortControlWidget::updatePortListSlot
    );
    connect(
        m_ui->autoConnectCheckBox, &QCheckBox::clicked,
        this, &SerialPortControlWidget::autoConnectChangedSlot
    );
}

void SerialPortControlWidget::setPort(QString portName)
{
    int index = m_ui->serialComboBox->findData(portName);
    if (index == -1) return;
    m_ui->serialComboBox->setCurrentIndex(index);
}

void SerialPortControlWidget::setBaudRate(unsigned int baudRate)
{
    int index = m_ui->baudComboBox->findData(baudRate);
    if (index == -1) return;
    m_ui->baudComboBox->setCurrentIndex(index);
}

void SerialPortControlWidget::setDataBits(QSerialPort::DataBits dataBits)
{
    int index = m_ui->dataBitsComboBox->findData(dataBits);
    if (index == -1) return;
    m_ui->dataBitsComboBox->setCurrentIndex(index);
}

void SerialPortControlWidget::setParity(QSerialPort::Parity parity)
{
    int index = m_ui->parityComboBox->findData(parity);
    if (index == -1) return;
    m_ui->parityComboBox->setCurrentIndex(index);
}

void SerialPortControlWidget::setStopBits(QSerialPort::StopBits stopBits)
{
    int index = m_ui->stopBitsComboBox->findData(stopBits);
    if (index == -1) return;
    m_ui->stopBitsComboBox->setCurrentIndex(index);
}

void SerialPortControlWidget::setFlowControl(QSerialPort::FlowControl flowControl)
{
    int index = m_ui->flowControlComboBox->findData(flowControl);
    if (index == -1) return;
    m_ui->flowControlComboBox->setCurrentIndex(index);
}

void SerialPortControlWidget::setValues(const SerialPortConfig& config)
{
    setPort(config.m_portName);
    setBaudRate(config.m_baudRate);
    setDataBits(config.m_dataBits);
    setParity(config.m_parity);
    setStopBits(config.m_stopBits);
    setFlowControl(config.m_flowControl);
}

void SerialPortControlWidget::setPortListUpdateMilliseconds(unsigned int milliseconds)
{
    m_updateMilliseconds = milliseconds;
    if (m_updateTimer->isActive())
    {
        m_updateTimer->stop();
        m_updateTimer->start(m_updateMilliseconds);
    }
}

QString SerialPortControlWidget::getPort() const
{
    return m_ui->serialComboBox->currentData().toString();
}

unsigned int SerialPortControlWidget::getBaudRate() const
{
    return m_ui->baudComboBox->currentData().toUInt();
}

QSerialPort::DataBits SerialPortControlWidget::getDataBits() const
{
    return QSerialPort::DataBits(m_ui->dataBitsComboBox->currentData().toInt());
}

QSerialPort::Parity SerialPortControlWidget::getParity() const
{
    return QSerialPort::Parity(m_ui->parityComboBox->currentData().toInt());
}

QSerialPort::StopBits SerialPortControlWidget::getStopBits() const
{
    return QSerialPort::StopBits(m_ui->stopBitsComboBox->currentData().toInt());
}

QSerialPort::FlowControl SerialPortControlWidget::getFlowControl() const
{
    return QSerialPort::FlowControl(m_ui->flowControlComboBox->currentData().toInt());
}

SerialPortConfig SerialPortControlWidget::getValues() const
{
    return SerialPortConfig(
        getPort(),
        m_autoconnectPort,
        getBaudRate(),
        getDataBits(),
        getParity(),
        getStopBits(),
        getFlowControl()
    );
}

void SerialPortControlWidget::portChangedSlot(int)
{
    changeToConnectToolButton();
    emit portChanged(getPort());
    emit updatePort(getValues());
}

void SerialPortControlWidget::baudRateChangedSlot(int)
{
    emit baudRateChanged(getBaudRate());
    emit updatePort(getValues());
}

void SerialPortControlWidget::dataBitsChangedSlot(int)
{
    emit dataBitsChanged(getDataBits());
    emit updatePort(getValues());
}

void SerialPortControlWidget::parityChangedSlot(int)
{
    emit parityChanged(getParity());
    emit updatePort(getValues());
}

void SerialPortControlWidget::stopBitsChangedSlot(int)
{
    emit stopBitsChanged(getStopBits());
    emit updatePort(getValues());
}

void SerialPortControlWidget::flowControlChangedSlot(int)
{
    emit flowControlChanged(getFlowControl());
    emit updatePort(getValues());
}

QString SerialPortControlWidget::generateSerialPortDetail(const QSerialPortInfo& serialPort)
{
    QString out = serialPort.portName();

    if(serialPort.hasVendorIdentifier() && serialPort.hasProductIdentifier())
    {
        SerialPortIdMatch match = findMatch(serialPort.vendorIdentifier(), serialPort.productIdentifier());
        bool hasData = false;

        if (!match.m_vendorName.empty())
        {
            out += QString(", '") + QString::fromStdString(match.m_vendorName) + QString("'");
            hasData = true;
        }

        if (!match.m_productName.empty())
        {
            out += QString(", ") + QString::fromStdString(match.m_productName);
            hasData = true;
        }
        else if(hasData && serialPort.hasProductIdentifier())
        {
            out += QString(", product Id: 0x%1").arg(serialPort.productIdentifier(), 4, 16, QLatin1Char('0'));
        }

        if (hasData) return out;
    }

    if(serialPort.hasVendorIdentifier())
    {
        out += QString(", vendor Id: 0x%1").arg(serialPort.vendorIdentifier(), 4, 16, QLatin1Char('0'));
    }

    if(serialPort.hasProductIdentifier())
    {
        out += QString(", product Id: 0x%1").arg(serialPort.productIdentifier(), 4, 16, QLatin1Char('0'));
    }

    return out;
}

bool SerialPortControlWidget::canAutoConnect()
{
    QString itemData;
    for (int i = 0 ; i < m_ui->serialComboBox->count() ; ++i)
    {
        itemData = m_ui->serialComboBox->itemData(i).toString();
        if (itemData == "") continue;
        if (itemData == m_autoconnectPort) return true;
    }
    return false;
}

bool SerialPortControlWidget::isPortListContainsPort(const QList<QSerialPortInfo>& serialPorts, const QString& portName)
{
    for (const QSerialPortInfo& serialPort : serialPorts)
    {
        if (!serialPort.hasVendorIdentifier())
        {
            // When a QSerialPort is connected to a serial port, then the device is disconnected
            // physically the QSerialPortInfo still reports that port as available.
            // But the vendor identitifier is empty. We rely on that behaviour here.
            continue;
        }
        if (portName == serialPort.portName()) return true;
    }
    return false;
}

void SerialPortControlWidget::updatePortListSlot()
{
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
    QString beforePort = getPort();

    QString portName;
    for (int i = 0 ; i < m_ui->serialComboBox->count() ; ++i)
    {
        portName = m_ui->serialComboBox->itemData(i).toString();
        if (portName == "") continue; // Keep empty item.
        if (!isPortListContainsPort(serialPorts, portName))
        {
            m_ui->serialComboBox->removeItem(i);
        }
    }

    for (const QSerialPortInfo& serialPort : serialPorts)
    {
        if (!serialPort.hasVendorIdentifier()) continue;
        if (m_ui->serialComboBox->findData(QVariant(serialPort.portName())) != -1) continue;

        QString serialName = generateSerialPortDetail(serialPort);
        m_ui->serialComboBox->addItem(serialName, QVariant(serialPort.portName()));
    }

    QString afterPort = getPort();

    if (afterPort.isEmpty() && canAutoConnect())
    {
        setPort(m_autoconnectPort);
        emit handlePort(getValues());
        return;
    }

    if (beforePort != afterPort)
    {
        setPort("");
        emit portChanged("");
    }
}

void SerialPortControlWidget::processReleasedSlot()
{
    emit handlePort(getValues());
}

void SerialPortControlWidget::deviceConnected()
{
    m_ui->autoConnectCheckBox->show();
    if (!m_autoconnectPort.isEmpty() && m_autoconnectPort == getPort())
    {
        m_ui->autoConnectCheckBox->setCheckState(Qt::CheckState::Checked);
    }
    else
    {
        m_ui->autoConnectCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }
    changeToDisconnectToolButton();
}

void SerialPortControlWidget::deviceDisconnected()
{
    m_ui->autoConnectCheckBox->hide();
    changeToConnectToolButton();
}

void SerialPortControlWidget::autoConnectChangedSlot(bool checked)
{
    if (checked) m_autoconnectPort = getPort();
    else m_autoconnectPort.clear();
}

void SerialPortControlWidget::showEvent(QShowEvent* event)
{
    m_updateTimer->start(m_updateMilliseconds);
    updatePortListSlot();
    QWidget::showEvent(event);
}

void SerialPortControlWidget::hideEvent(QHideEvent* event)
{
    m_updateTimer->stop();
    QWidget::hideEvent(event);
}

void SerialPortControlWidget::changeToDisconnectToolButton()
{
    m_ui->processToolButton->setStyleSheet("QToolButton{color:black;}");
    m_ui->processToolButton->setText("Close");
    m_ui->processToolButton->setEnabled(true);
}

void SerialPortControlWidget::changeToConnectToolButton()
{
    m_ui->processToolButton->setText("Open");
    if (getPort().isEmpty())
    {
        m_ui->processToolButton->setStyleSheet("QToolButton{color:red;}");
        m_ui->processToolButton->setEnabled(false);
        m_ui->autoConnectCheckBox->hide();
    }
    else
    {
        m_ui->processToolButton->setStyleSheet("QToolButton{color:green;}");
        m_ui->processToolButton->setEnabled(true);
    }
}
