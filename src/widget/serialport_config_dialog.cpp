#include "serialport_config_dialog.h"
#include "./ui_serialport_config_dialog.h"

SerialPortConfigDialog::SerialPortConfigDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::SerialPortConfigDialog)
{
    m_ui->setupUi(this);

    m_ui->dataBitsComboBox->addItem("5", QSerialPort::DataBits::Data5);
    m_ui->dataBitsComboBox->addItem("6", QSerialPort::DataBits::Data6);
    m_ui->dataBitsComboBox->addItem("7", QSerialPort::DataBits::Data7);
    m_ui->dataBitsComboBox->addItem("8", QSerialPort::DataBits::Data8);

    setDataBits(QSerialPort::DataBits::Data8);

    m_ui->parityComboBox->addItem("None", QSerialPort::Parity::NoParity);
    m_ui->parityComboBox->addItem("Even", QSerialPort::Parity::EvenParity);
    m_ui->parityComboBox->addItem("Odd", QSerialPort::Parity::OddParity);
    m_ui->parityComboBox->addItem("Space", QSerialPort::Parity::SpaceParity);
    m_ui->parityComboBox->addItem("Mark", QSerialPort::Parity::MarkParity);

    setParity(QSerialPort::Parity::NoParity);

    m_ui->stopBitsComboBox->addItem("1", QSerialPort::StopBits::OneStop);
    m_ui->stopBitsComboBox->addItem("1.5", QSerialPort::StopBits::OneAndHalfStop);
    m_ui->stopBitsComboBox->addItem("2", QSerialPort::StopBits::TwoStop);

    setStopBits(QSerialPort::StopBits::OneStop);

    m_ui->flowControlComboBox->addItem("None", QSerialPort::FlowControl::NoFlowControl);
    m_ui->flowControlComboBox->addItem("Hardware", QSerialPort::FlowControl::HardwareControl);
    m_ui->flowControlComboBox->addItem("Software", QSerialPort::FlowControl::SoftwareControl);

    setFlowControl(QSerialPort::FlowControl::NoFlowControl);

    connect(
        m_ui->dataBitsComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(dataBitsChangedSlot(int))
    );

    connect(
        m_ui->parityComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(parityChangedSlot(int))
    );

    connect(
        m_ui->stopBitsComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(stopBitsChangedSlot(int))
    );

    connect(
        m_ui->flowControlComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(flowControlChangedSlot(int))
    );

    connect(
        m_ui->saveToolButton, SIGNAL(released()),
        this, SLOT(accept())
    );
}

SerialPortConfigDialog::~SerialPortConfigDialog()
{
    delete m_ui;
}

Ui::SerialPortConfigDialog* SerialPortConfigDialog::ui()
{
    return m_ui;
}

void SerialPortConfigDialog::setDataBits(QSerialPort::DataBits dataBits)
{
    int index = m_ui->dataBitsComboBox->findData(dataBits);
    if (index == -1) return;
    m_ui->dataBitsComboBox->setCurrentIndex(index);
}

void SerialPortConfigDialog::setParity(QSerialPort::Parity parity)
{
    int index = m_ui->parityComboBox->findData(parity);
    if (index == -1) return;
    m_ui->parityComboBox->setCurrentIndex(index);
}

void SerialPortConfigDialog::setStopBits(QSerialPort::StopBits stopBits)
{
    int index = m_ui->stopBitsComboBox->findData(stopBits);
    if (index == -1) return;
    m_ui->stopBitsComboBox->setCurrentIndex(index);
}

void SerialPortConfigDialog::setFlowControl(QSerialPort::FlowControl flowControl)
{
    int index = m_ui->flowControlComboBox->findData(flowControl);
    if (index == -1) return;
    m_ui->flowControlComboBox->setCurrentIndex(index);
}

QSerialPort::DataBits SerialPortConfigDialog::getDataBits()
{
    return QSerialPort::DataBits(m_ui->dataBitsComboBox->currentData().toInt());
}

QSerialPort::Parity SerialPortConfigDialog::getParity()
{
    return QSerialPort::Parity(m_ui->parityComboBox->currentData().toInt());
}

QSerialPort::StopBits SerialPortConfigDialog::getStopBits()
{
    return QSerialPort::StopBits(m_ui->stopBitsComboBox->currentData().toInt());
}

QSerialPort::FlowControl SerialPortConfigDialog::getFlowControl()
{
    return QSerialPort::FlowControl(m_ui->flowControlComboBox->currentData().toInt());
}

void SerialPortConfigDialog::dataBitsChangedSlot(int)
{
    emit dataBitsChanged(getDataBits());
}

void SerialPortConfigDialog::parityChangedSlot(int)
{
    emit parityChanged(getParity());
}

void SerialPortConfigDialog::stopBitsChangedSlot(int)
{
    emit stopBitsChanged(getStopBits());
}

void SerialPortConfigDialog::flowControlChangedSlot(int)
{
    emit flowControlChanged(getFlowControl());
}
