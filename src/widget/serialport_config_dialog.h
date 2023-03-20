#pragma once

#include <QDialog>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class SerialPortConfigDialog; }
QT_END_NAMESPACE

class SerialPortConfigDialog : public QDialog
{
    Q_OBJECT

public:
    SerialPortConfigDialog(QWidget *parent = nullptr);
    ~SerialPortConfigDialog();

    Ui::SerialPortConfigDialog* ui();

    void setDataBits(QSerialPort::DataBits dataBits);
    void setParity(QSerialPort::Parity parity);
    void setStopBits(QSerialPort::StopBits stopBits);
    void setFlowControl(QSerialPort::FlowControl flowControl);

    QSerialPort::DataBits getDataBits();
    QSerialPort::Parity getParity();
    QSerialPort::StopBits getStopBits();
    QSerialPort::FlowControl getFlowControl();

signals:
    void dataBitsChanged(QSerialPort::DataBits);
    void parityChanged(QSerialPort::Parity);
    void stopBitsChanged(QSerialPort::StopBits);
    void flowControlChanged(QSerialPort::FlowControl);

private slots:
    void dataBitsChangedSlot(int);
    void parityChangedSlot(int);
    void stopBitsChangedSlot(int);
    void flowControlChangedSlot(int);

private:
    Ui::SerialPortConfigDialog *m_ui;
};
