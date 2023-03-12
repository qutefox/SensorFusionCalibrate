#include "application.h"
#include "ui_mainwindow.h"

Application::Application(int argc, char *argv[])
    : QApplication{argc, argv}
    , m_timer{new QTimer(this)}
    , m_mainWindow{new MainWindow()}
    , m_serial{new QSerialPort(this)}
    , m_ansi{new AnsiEscape(this)}
{

    QComboBox* serialList = m_mainWindow->ui()->serialList;
    serialList->addItem("", QVariant("")); // Adding empty item so the user has to choose a port.

    connect(
        serialList, SIGNAL(currentTextChanged(QString)),
        this, SLOT(serialChanged(QString))
    );

    QComboBox* baudList = m_mainWindow->ui()->baudList;
    for (const qint32 baudRate : QSerialPortInfo::standardBaudRates())
    {
        baudList->addItem(QString::number(baudRate), QVariant(baudRate));
    }
    int baudIndex = baudList->findData(115200);
    if (baudIndex != -1) baudList->setCurrentIndex(baudIndex);

    connect(
        baudList, SIGNAL(currentTextChanged(QString)),
        this, SLOT(serialChanged(QString))
    );

    connect(
        m_mainWindow->ui()->serialOpenCloseButton, SIGNAL(clicked(bool)),
        this, SLOT(serialOpenCloseClicked(bool))
    );

    m_mainWindow->ui()->terminalPlainTextEdit->setAnsiEscape(m_ansi);

    connect(
        m_serial, SIGNAL(readyRead()),
        this, SLOT(serialReadyRead())
    );

    connect(
        m_serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
        this, SLOT(serialOnError(QSerialPort::SerialPortError))
    );

    connect(
        m_serial, SIGNAL(aboutToClose()),
        this, SLOT(serialOnClose())
    );

    connect(
        m_mainWindow->ui()->terminalScrollCheckbox, SIGNAL(stateChanged(int)),
        m_mainWindow->ui()->terminalPlainTextEdit, SLOT(scrollToBottom(int))
    );

    connect(
        m_mainWindow->ui()->terminalPlainTextEdit, SIGNAL(writeData(QByteArray)),
        this, SLOT(terminalWriteData(QByteArray))
    );

    connect(
        m_mainWindow->ui()->terminalPlainTextEdit, SIGNAL(processLine(QString)),
        this, SLOT(processInputLine(QString))
    );

    connect(
        m_ansi, SIGNAL(updateWindowTitle(QString)),
        this, SLOT(updateWindowTitle(QString))
    );

    updateUi();
    serialChanged("");

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateUi()));
    m_timer->start(1000);

    m_mainWindow->show();
}

Application::~Application()
{
    delete m_mainWindow;
    m_serial->close();
}

void Application::updateUi()
{
    updateSerialList();
}

QString Application::GenerateReadableSerialPortName(const QSerialPortInfo& serialPort)
{
    QString out = serialPort.portName();
    if(serialPort.hasVendorIdentifier())
    {
        out += QString(", VendorId: 0x%1").arg(serialPort.vendorIdentifier(), 4, 16, QLatin1Char( '0' ));
    }
    if(serialPort.hasProductIdentifier())
    {
        out += QString(", ProductId: 0x%1").arg(serialPort.productIdentifier(), 4, 16, QLatin1Char( '0' ));
    }
    return out;
}

void Application::updateSerialList()
{
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
    QComboBox* serialList = m_mainWindow->ui()->serialList;

    QString prevPort = serialList->itemData(serialList->currentIndex()).toString();

    QString itemData;
    bool found = false;
    for (int i = 0 ; i < serialList->count() ; ++i)
    {
        itemData = serialList->itemData(i).toString();
        if (itemData == "") continue; // keep empty item

        found = false;
        for (const QSerialPortInfo& serialPort : serialPorts)
        {
            if (!serialPort.hasVendorIdentifier()) break;
            if (itemData == serialPort.portName())
            {
                found = true;
                break;
            }
        }
        if (!found) serialList->removeItem(i);
    }

    for (const QSerialPortInfo& serialPort : serialPorts)
    {
        if (!serialPort.hasVendorIdentifier()) continue;
        if (serialList->findData(QVariant(serialPort.portName())) != -1) continue;

        QString serialName = GenerateReadableSerialPortName(serialPort);
        serialList->addItem(serialName, QVariant(serialPort.portName()));
    }

    QString afterPort = serialList->itemData(serialList->currentIndex()).toString();

    if (prevPort != afterPort)
    {
        int portIndex = serialList->findData("");
        if (portIndex != -1) serialList->setCurrentIndex(portIndex);

        serialChanged("");
    }

    if (m_serialOpen != m_serial->isOpen())
    {
        serialChanged(afterPort);
    }
}

void Application::serialChanged(QString newText)
{
    Q_UNUSED(newText)

    QPushButton* button = m_mainWindow->ui()->serialOpenCloseButton;
    QComboBox* serialList = m_mainWindow->ui()->serialList;
    QString selectedSerialPort = serialList->itemData(serialList->currentIndex()).toString();

    if (!m_serial->isOpen())
    {
        if (selectedSerialPort == "")
        {
            button->setText("Select a port");
            button->setStyleSheet("QPushButton {color:black;}");
            button->setDisabled(true);
        }
        else
        {
            button->setText("Open");
            button->setStyleSheet("QPushButton {color:green;}");
            button->setDisabled(false);
        }
        return;
    }

    if (selectedSerialPort != m_serial->portName())
    {
        m_serial->close();
        m_serialOpen = false;

        if (selectedSerialPort == "")
        {
            button->setText("Select a port");
            button->setStyleSheet("QPushButton {color:black;}");
            button->setDisabled(true);
        }
        else
        {
            button->setText("Open");
            button->setStyleSheet("QPushButton {color:green;}");
            button->setDisabled(false);
        }
        return;
    }

    QComboBox* baudList = m_mainWindow->ui()->baudList;
    int selectedBaudRate = baudList->itemData(baudList->currentIndex()).toInt();

    if (selectedBaudRate != m_serial->baudRate())
    {
        m_serial->setBaudRate(selectedBaudRate);
    }
}

void Application::serialOpenCloseClicked(bool checked)
{
    Q_UNUSED(checked)

    QPushButton* button = m_mainWindow->ui()->serialOpenCloseButton;
    QComboBox* serialList = m_mainWindow->ui()->serialList;
    QString selectedSerialPort = serialList->itemData(serialList->currentIndex()).toString();
    QLabel* statusLabel = m_mainWindow->statusLabel();

    if (selectedSerialPort.isEmpty())
    {
        m_serial->close();
        m_serialOpen = false;

        button->setText("Select a port");
        button->setStyleSheet("QPushButton {color:black;}");
        button->setDisabled(true);
        return;
    }

    if (m_serial->isOpen())
    {
        m_serial->close();
        m_serialOpen = false;

        button->setText("Open");
        button->setStyleSheet("QPushButton {color:green;}");
        button->setDisabled(false);
        statusLabel->setText(QString("Closed serial port: %1.").arg(m_serial->portName()));
    }
    else
    {
        QComboBox* baudList = m_mainWindow->ui()->baudList;
        int selectedBaudRate = baudList->itemData(baudList->currentIndex()).toInt();

        m_serial->setPortName(selectedSerialPort);
        m_serial->setBaudRate(selectedBaudRate);
        m_serial->setDataBits(QSerialPort::DataBits::Data8);
        m_serial->setParity(QSerialPort::Parity::NoParity);
        m_serial->setStopBits(QSerialPort::StopBits::OneStop);
        m_serial->setFlowControl(QSerialPort::FlowControl::NoFlowControl);

        if (m_serial->open(QIODevice::ReadWrite))
        {
            m_serialOpen = true;

            button->setText("Close");
            button->setStyleSheet("QPushButton {color:red;}");
            button->setDisabled(false);
            statusLabel->setText(QString("Opened serial port: %1.").arg(m_serial->portName()));
        }
        else
        {
            m_serialOpen = false;

            button->setText("Open");
            button->setStyleSheet("QPushButton {color:green;}");
            button->setDisabled(false);
            statusLabel->setText(QString("Failed to open serial port: %1.").arg(m_serial->portName()));
        }
    }
}


void Application::serialReadyRead()
{
    SerialTerminal* terminal = m_mainWindow->ui()->terminalPlainTextEdit;
    terminal->putData(m_serial->readAll());
}

void Application::serialOnError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::SerialPortError::NoError) return;

    QLabel* statusLabel = m_mainWindow->statusLabel();
    statusLabel->setText(QString("Serial '%1' error: %2").arg(m_serial->portName(), m_serial->errorString()));
}

void Application::serialOnClose()
{
    m_mainWindow->setWindowTitle("Sensor Fusion Calibrate");
}

void Application::updateWindowTitle(QString title)
{
    QString newTitle("Sensor Fusion Calibrate");
    if (!title.isEmpty()) newTitle += QString(" - ") + title;
    m_mainWindow->setWindowTitle(newTitle);
}

void Application::terminalWriteData(QByteArray data)
{
    if (m_serial->isOpen())
    {
        m_serial->write(data);
    }
}

void Application::processInputLine(QString line)
{
    qDebug() << QString("Process line: '%1'").arg(line) << Qt::endl;

    QList<double> numbers;
    double number;
    QString possibleNumber;
    bool conversionSuccess = true;
    int dotCount = 0;
    for (const QChar c : line)
    {
        if (c.isDigit())
        {
            possibleNumber += c;
        }
        else if (c == '.')
        {
            if (dotCount == 0)
            {
                possibleNumber += c;
                ++dotCount;
            }
            else
            {
                number = possibleNumber.toDouble(&conversionSuccess);
                if (conversionSuccess) numbers.push_back(number);
                possibleNumber.clear();
                dotCount = 0;
            }
        }
        else if (!possibleNumber.isEmpty())
        {
            number = possibleNumber.toDouble(&conversionSuccess);
            if (conversionSuccess) numbers.push_back(number);
            possibleNumber.clear();
            dotCount = 0;
        }
    }

    if (!possibleNumber.isEmpty())
    {
        number = possibleNumber.toDouble(&conversionSuccess);
        if (conversionSuccess) numbers.push_back(number);
        possibleNumber.clear();
        dotCount = 0;
    }

    if (numbers.size() >= 3)
    {
        DataItem dataItem(numbers[0], numbers[1], numbers[2]);
        qDebug() << dataItem.toString() << Qt::endl;
        m_dataItems.push_back(dataItem);
    }
}
