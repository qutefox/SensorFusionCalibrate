#pragma once

#include <set>
#include <fstream>

#include <QString>

#include "idatasource.h"

class CSVFileDataSource : IDataSource
{

public:
    CSVFileDataSource(const QString& filePath);
    virtual ~CSVFileDataSource();

    std::set<Point>&& getNextPoints(long long maxNumberOfPoints = -1) override;
    long long totalNumberOfPoints() override;

private:
    QString m_filePath;
    std::ifstream m_csvFile;
    std::set<Point> m_points;
    long long m_totalNumberOfPoints = -1;

    void fillBuffer(long long maxNumberOfPoints);
    bool shouldReadMore(long long maxNumberOfPoints);
};
