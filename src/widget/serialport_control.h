#pragma once

#include <QSerialPort>
#include <QWidget>
#include <QTimer>

#include <data/serialport_config.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SerialPortControlWidget; }
QT_END_NAMESPACE

class SerialPortControlWidget : public QWidget
{
    Q_OBJECT

public:
    SerialPortControlWidget(const SerialPortConfig& config, QWidget *parent = nullptr);
    virtual ~SerialPortControlWidget();

    void setPort(QString portName);
    void setBaudRate(unsigned int baudRate);
    void setDataBits(QSerialPort::DataBits dataBits);
    void setParity(QSerialPort::Parity parity);
    void setStopBits(QSerialPort::StopBits stopBits);
    void setFlowControl(QSerialPort::FlowControl flowControl);
    void setValues(const SerialPortConfig& config);
    void setPortListUpdateMilliseconds(unsigned int milliseconds);

    QString getPort() const;
    unsigned int getBaudRate() const;
    QSerialPort::DataBits getDataBits() const;
    QSerialPort::Parity getParity() const;
    QSerialPort::StopBits getStopBits() const;
    QSerialPort::FlowControl getFlowControl() const;
    SerialPortConfig getValues() const;

signals:
    void portChanged(QString);
    void baudRateChanged(uint);
    void dataBitsChanged(QSerialPort::DataBits);
    void parityChanged(QSerialPort::Parity);
    void stopBitsChanged(QSerialPort::StopBits);
    void flowControlChanged(QSerialPort::FlowControl);
    void handlePort(SerialPortConfig);
    void updatePort(SerialPortConfig);

public slots:
    void deviceConnected();
    void deviceDisconnected();

private slots:
    void portChangedSlot(int);
    void baudRateChangedSlot(int);
    void dataBitsChangedSlot(int);
    void parityChangedSlot(int);
    void stopBitsChangedSlot(int);
    void flowControlChangedSlot(int);
    void processReleasedSlot();
    void updatePortListSlot();
    void autoConnectChangedSlot(bool);

private:
    QTimer* m_updateTimer = nullptr;
    Ui::SerialPortControlWidget* m_ui = nullptr;
    unsigned int m_updateMilliseconds = 3000;
    QString m_autoconnectPort;

    void setupDropdownValues();
    void setupSignalSlot();
    QString generateSerialPortDetail(const QSerialPortInfo& serialPort);
    bool isPortListContainsPort(const QList<QSerialPortInfo>& serialPorts, const QString& portName);
    bool canAutoConnect();
    void changeToDisconnectToolButton();
    void changeToConnectToolButton();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
};
