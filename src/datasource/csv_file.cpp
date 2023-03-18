#include "csv_file.h"

#include <QLabel>
#include <QAction>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>

const QRegularExpression CSVFileDataSource::numRegex = QRegularExpression(R"(.*?(-?)([\d]+)(\.[\d]+)?.*)");

CSVFileDataSource::CSVFileDataSource(QWidget* parent)
    : IDataSource{ parent }
{
    m_toolButtonProcess =  new QToolButton();
    m_toolButtonProcess->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolButtonProcess->setIcon(QIcon(":/images/process-black.png"));
    m_toolButtonProcess->setText("Process");
    m_toolButtonProcess->setEnabled(false);

    m_lineEdit = new QLineEdit();

    m_toolButtonChoose = new QToolButton();
    m_toolButtonChoose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolButtonChoose->setIcon(QIcon(":/images/open-file-black.png"));
    m_toolButtonChoose->setText("Choose file");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_toolButtonProcess);
    layout->addWidget(new QLabel("File path: "));
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_toolButtonChoose);

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
        m_toolButtonProcess->setEnabled(true);
    }
    else
    {
        m_toolButtonProcess->setIcon(QIcon(":/images/fail-darkred.png"));
        m_toolButtonProcess->setText("Invalid file");
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
    m_toolButtonProcess->setText("Cancel");
    m_lineEdit->setEnabled(false);
    m_toolButtonChoose->setEnabled(false);
    emit opened();
}

void CSVFileDataSource::processDone()
{
    m_csvFile.close();
    m_toolButtonProcess->setIcon(QIcon(":/images/process-black.png"));
    m_toolButtonProcess->setText("Process");
    m_lineEdit->setEnabled(true);
    m_toolButtonChoose->setEnabled(true);
    emit closed();
}

void CSVFileDataSource::processFailed()
{
    m_toolButtonProcess->setIcon(QIcon(":/images/fail-darkred.png"));
    m_toolButtonProcess->setText("Failed");
    m_toolButtonProcess->setEnabled(false);
    m_lineEdit->setEnabled(true);
    m_toolButtonChoose->setEnabled(true);
    emit failedToOpen(m_csvFile.errorString());
}

bool CSVFileDataSource::getNextPoints(QSet<Point>& dev0, QSet<Point>& dev1, QSet<Point>& dev2)
{
    dev0.clear();
    dev1.clear();
    dev2.clear();

    if (!m_csvFile.isOpen()) return false;

    QString line, doubleStr;
    QStringList row;
    QList<double> doubles;
    QRegularExpressionMatch match;
    bool doubleConvertedOk;
    double doubleNum;
    Point point;
    QTextStream in(&m_csvFile);
    while (!in.atEnd())
    {
        line = in.readLine();
        if (line.isEmpty() && !m_csvFile.isOpen()) break;
        row = line.split(',');
        for (const QString& item : row)
        {
            match = CSVFileDataSource::numRegex.match(item);
            if (match.hasMatch())
            {
                doubleStr.clear();
                if (match.capturedLength()>=1) doubleStr += match.captured(0);
                if (match.capturedLength()>=2) doubleStr += match.captured(1);
                if (match.capturedLength()>=3) doubleStr += match.captured(2);
                doubleNum = doubleStr.toDouble(&doubleConvertedOk);
                if (doubleConvertedOk) doubles.push_back(doubleNum);
            }
        }
        if (doubles.size() >= 3) dev0.insert(Point(doubles[0], doubles[1], doubles[2]));
        if (doubles.size() >= 6) dev1.insert(Point(doubles[3], doubles[4], doubles[5]));
        if (doubles.size() >= 9) dev1.insert(Point(doubles[6], doubles[7], doubles[8]));
    }

    processDone();
    return true;
}

