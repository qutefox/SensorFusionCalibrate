#pragma once

#include <set>
#include <memory>
#include <fstream>

#include <QString>

#include "idatasource.h"

class CSVFileDataSource : IDataSource
{

public:
    CSVFileDataSource(const QString& filePath = "");
    virtual ~CSVFileDataSource();

    std::set<Point>&& getNextPoints() override;
    bool applyConfig(const QMap<QString, QVariant>& config) override;
    bool isStream() const override { return false; }

private:
    QString m_filePath;
    std::unique_ptr<std::ifstream> m_csvFile;
    std::set<Point> m_points;

    void fillBuffer();
    void openFile(const QString& filePath);
};
