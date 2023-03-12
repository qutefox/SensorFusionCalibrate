#pragma once

#include "mainwindow.h"
#include "dataitem.h"
#include "ansiescape.h"

#include <QApplication>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char *argv[]);
    virtual ~Application();

    void updateSerialList();

private slots:
    void updateUi();
    void serialChanged(QString);
    void serialOpenCloseClicked(bool);
    void serialReadyRead();
    void serialOnError(QSerialPort::SerialPortError);
    void serialOnClose();
    void terminalWriteData(QByteArray);
    void processInputLine(QString);
    void updateWindowTitle(QString);

private:

    QString GenerateReadableSerialPortName(const QSerialPortInfo& serialPort);

    QTimer* m_timer = nullptr;
    MainWindow* m_mainWindow = nullptr;
    QSerialPort* m_serial = nullptr;
    AnsiEscape* m_ansi = nullptr;
    bool m_serialOpen = false;
    QList<DataItem> m_dataItems;
};

