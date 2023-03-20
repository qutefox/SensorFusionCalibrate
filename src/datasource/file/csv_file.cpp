#include "csv_file.h"

#include <QLabel>
#include <QAction>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>

#include <datasource/line_parser.h>

CSVFileDataSource::CSVFileDataSource(const QString& filePath, QObject* parent)
    : IDataSource{ parent }
    , m_widget{ new CSVFileControlWidget(filePath) }
{
    connect(
        m_widget, &CSVFileControlWidget::processFile,
        this, &CSVFileDataSource::processFile
    );
}

CSVFileDataSource::~CSVFileDataSource()
{
    if(m_csvFile.isOpen())
    {
        m_csvFile.close();
    }
    delete m_widget;
}

QWidget* CSVFileDataSource::widget() const
{
    return m_widget;
}

void CSVFileDataSource::processFile(QString filePath)
{
    if (m_csvFile.isOpen())
    {
        emit dataSourceEnded();
        return;
    }

    m_csvFile.setFileName(filePath);
    if (m_csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit dataSourceStarted();
        return;
    }

    emit errorOccurred(m_csvFile.errorString());
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

    emit dataSourceEnded();
    return gotData;
}

