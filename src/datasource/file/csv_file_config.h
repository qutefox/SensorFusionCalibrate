#pragma once

#include <QString>

class CSVFileConfig
{
public:
    QString m_filePath;
    QString m_browseDirectoryPath;

    CSVFileConfig();
    CSVFileConfig(const QString& filePath, const QString& browseDirectoryPath);

    void resetToDefault();
};
