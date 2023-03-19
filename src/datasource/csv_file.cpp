#include "csv_file.h"

#include <QLabel>
#include <QAction>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>

#include "parse_numbers.h"

const QRegularExpression CSVFileDataSource::numRegex = QRegularExpression(R"((.*?((-?)([\d]+)(\.[\d]+)?)))");

CSVFileDataSource::CSVFileDataSource(QWidget* parent)
    : IDataSource{ parent }
{
    m_lineEdit = new QLineEdit();

    m_toolButtonChoose = new QToolButton();
    m_toolButtonChoose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolButtonChoose->setIcon(QIcon(":/images/open-black.png"));
    m_toolButtonChoose->setText("Choose file");

    m_toolButtonProcess =  new QToolButton();
    m_toolButtonProcess->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolButtonProcess->setIcon(QIcon(":/images/process-black.png"));
    m_toolButtonProcess->setStyleSheet("QToolButton{color:black;}");
    m_toolButtonProcess->setText("Process");
    m_toolButtonProcess->setEnabled(false);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("File path: "));
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_toolButtonChoose);
    layout->addWidget(m_toolButtonProcess);
    // layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    connect(
        m_toolButtonChoose, SIGNAL(released()),
        this, SLOT(openFileDialog())
    );

    connect(
        m_lineEdit, SIGNAL(textChanged(QString)),
        this, SLOT(isFilePathValid(QString))
    );

    connect(
        m_toolButtonProcess, SIGNAL(released()),
        this, SLOT(processFile())
    );
}

CSVFileDataSource::~CSVFileDataSource()
{
    if(m_csvFile.isOpen())
    {
        m_csvFile.close();
    }
}

void CSVFileDataSource::openFileDialog()
{
    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");

    QString filePath =  QFileDialog::getOpenFileName(
        this,
        "Open CSV File",
        QDir::currentPath(),
        filters, &defaultFilter
    );

    m_lineEdit->setText(filePath);
}

void CSVFileDataSource::isFilePathValid(QString filePath)
{
    QFileInfo checkFile(filePath);
    if (checkFile.exists() && checkFile.isFile())
    {
        m_toolButtonProcess->setIcon(QIcon(":/images/process-black.png"));
        m_toolButtonProcess->setText("Process");
        m_toolButtonProcess->setStyleSheet("QToolButton{color:green;}");
        m_toolButtonProcess->setEnabled(true);
    }
    else
    {
        m_toolButtonProcess->setIcon(QIcon(":/images/fail-darkred.png"));
        m_toolButtonProcess->setText("Invalid file");
        m_toolButtonProcess->setStyleSheet("QToolButton{color:red;}");
        m_toolButtonProcess->setEnabled(false);
    }
}

void CSVFileDataSource::processFile()
{
    if (m_csvFile.isOpen())
    {
        processDone();
        return;
    }

    m_csvFile.setFileName(m_lineEdit->text());
    if (m_csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        processStarted();
        return;
    }

    processFailed();
}

void CSVFileDataSource::processStarted()
{
    m_toolButtonProcess->setIcon(QIcon(":/images/cancel-darkred.png"));
    m_toolButtonProcess->setStyleSheet("QToolButton{color:orange;}");
    m_toolButtonProcess->setText("Cancel");
    m_lineEdit->setEnabled(false);
    m_toolButtonChoose->setEnabled(false);
    emit opened();
}

void CSVFileDataSource::processDone()
{
    m_csvFile.close();
    m_toolButtonProcess->setIcon(QIcon(":/images/process-black.png"));
    m_toolButtonProcess->setStyleSheet("QToolButton{color:black;}");
    m_toolButtonProcess->setText("Process");
    m_lineEdit->setEnabled(true);
    m_toolButtonChoose->setEnabled(true);
    emit closed();
}

void CSVFileDataSource::processFailed()
{
    m_toolButtonProcess->setIcon(QIcon(":/images/fail-darkred.png"));
    m_toolButtonProcess->setStyleSheet("QToolButton{color:red;}");
    m_toolButtonProcess->setText("Failed");
    m_toolButtonProcess->setEnabled(false);
    m_lineEdit->setEnabled(true);
    m_toolButtonChoose->setEnabled(true);
    emit failed(m_csvFile.errorString());
}

bool CSVFileDataSource::getNextPoints(std::vector<std::set<Point>>& devicePoints)
{
    devicePoints.clear();

    if (!m_csvFile.isOpen()) return false;

    bool gotData = false;
    QString line;
    QTextStream in(&m_csvFile);

    while (!in.atEnd())
    {
        line = in.readLine();
        if (line.isEmpty() && !m_csvFile.isOpen()) break;

        gotData |= parseLineToDeviceData(line.toStdString(), devicePoints);
    }

    processDone();
    return gotData;
}

