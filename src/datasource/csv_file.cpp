#include "csv_file.h"

#include <sstream>
#include <string>
#include <exception>

CSVFileDataSource::CSVFileDataSource(const QString& filePath)
{
    m_csvFile.open(filePath.toStdString());
    if (!m_csvFile.is_open())
    {
        std::ostringstream oss;
        oss << "Could not open file: '" << filePath.toStdString() << "'.";
        throw std::ios_base::failure(oss.str());
    }

    std::string line;
    while (std::getline(m_csvFile, line))
    {
        ++m_totalNumberOfPoints;
    }

    m_csvFile.clear();
    m_csvFile.seekg(0);
}

CSVFileDataSource::~CSVFileDataSource()
{
    m_csvFile.close();
}

bool CSVFileDataSource::shouldReadMore(long long maxNumberOfPoints)
{
    if (m_csvFile.eof()) return false;
    if (maxNumberOfPoints <= 0 || m_points.size() < maxNumberOfPoints) return true;
    return false;
}

void CSVFileDataSource::fillBuffer(long long maxNumberOfPoints)
{
    std::string line;
    while(shouldReadMore(maxNumberOfPoints) && getline(m_csvFile, line))
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

std::set<Point>&& CSVFileDataSource::getNextPoints(long long maxNumberOfPoints)
{
    fillBuffer(maxNumberOfPoints);
    return std::move(m_points);
}

long long CSVFileDataSource::totalNumberOfPoints()
{
    return m_totalNumberOfPoints;
}
