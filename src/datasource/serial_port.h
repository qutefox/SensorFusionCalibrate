#pragma once

#include <set>

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QToolButton>
#include <QTimer>

#include "idatasource.h"
#include "../widget/serialport_config_dialog.h"

class SerialPortDataSource : public IDataSource
{
    Q_OBJECT

public:
    SerialPortDataSource(QWidget* parent = nullptr);
    virtual ~SerialPortDataSource();

    static QString getName() { return "Serial Port"; }
    bool getNextPoints(std::vector<std::set<Point>>& points) override;
    bool isStream() const override { return true; }

private slots:
    void update();
    void serialPortChanged(QString newText);
    void readyRead();
    void processButtonReleased();
    void onSerialError(QSerialPort::SerialPortError);
    void exitReadLoop();
    void openConfigWindow();
    void saveSerialConfig();

private:
    SerialPortConfigDialog* m_configDialog;
    QComboBox* m_portComboBox;
    QComboBox* m_baudComboBox;
    QToolButton* m_toolButtonConfig;
    QToolButton* m_toolButtonProcess;
    QTimer* m_timer = nullptr;
    QTimer* m_readLoopTimer = nullptr;
    bool m_serialOpen = false;
    bool m_exitReadLoop = false;
    QSerialPort m_serial;

    QString GenerateReadableSerialPortName(const QSerialPortInfo& serialPort);
    void updateSerialList();
    QString currentPortComboBoxString() const;
};

