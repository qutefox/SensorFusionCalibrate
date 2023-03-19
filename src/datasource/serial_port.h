#pragma once

#include <set>

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QToolButton>
#include <QTimer>

#include "idatasource.h"


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

private:
    QComboBox* m_portComboBox;
    QComboBox* m_baudComboBox;
    QToolButton* m_toolButtonConfig;
    QToolButton* m_toolButtonProcess;
    QTimer* m_timer;
    bool m_serialOpen = false;
    QSerialPort m_serial;

    QString GenerateReadableSerialPortName(const QSerialPortInfo& serialPort);
    void updateSerialList();
    QString currentPortComboBoxString() const;
};

