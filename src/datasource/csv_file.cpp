#include "csv_file.h"

#include <sstream>
#include <string>
#include <exception>

CSVFileDataSource::CSVFileDataSource(const QString& filePath)
{
    openFile(filePath);
}

CSVFileDataSource::~CSVFileDataSource()
{
    if (m_csvFile) m_csvFile->close();
    m_csvFile.reset();
}

void CSVFileDataSource::openFile(const QString& filePath)
{
    if (m_csvFile) m_csvFile->close();
    m_csvFile.reset();

    if (filePath.isEmpty()) return;

    // Set exceptions to be thrown on failure
    m_csvFile->exceptions(std::ifstream::failbit | std::ifstream::badbit);
    m_csvFile->open(filePath.toStdString(), std::ifstream::in);

    /*
    std::size_t lineCount = 0;
    std::string line;
    while (std::getline(*m_csvFile, line))
    {
        ++lineCount;
    }

    m_csvFile->clear();
    m_csvFile->seekg(0);

    Since we use and std::set we don't need to pre-allocate the memory.

    If we had used an std::vector as container we would have wanted to do:
    std::vector<Point> p; p.reserve(lineCounts);
    to gain a little performance boost.
    */
}

void CSVFileDataSource::fillBuffer()
{
    if (!m_csvFile) return;

    std::string line;
    while(getline(*m_csvFile, line))
    {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string lineStream;
        std::vector<double> row;
        while(getline(iss, lineStream, ',') && row.size() < 3)
        {
            row.push_back(std::stod(lineStream));
        }
        if (row.size() < 3) continue;
        m_points.insert(Point(row[0], row[1], row[2]));
    }
}

std::set<Point>&& CSVFileDataSource::getNextPoints()
{
    fillBuffer();
    return std::move(m_points);
}

bool CSVFileDataSource::applyConfig(const QMap<QString, QVariant>& config)
{
    QVariant fileNameVariant = config.value("fileName", QVariant(""));
    QString fileName = fileNameVariant.toString();
    if (!fileName.isEmpty())
    {
        openFile(fileName);
        return true;
    }

    return false;
}
