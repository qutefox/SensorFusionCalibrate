#include "csv_file_config.h"

#include <QDir>

CSVFileConfig::CSVFileConfig()
    : m_filePath{ "" }
    , m_browseDirectoryPath{ QDir::currentPath() }
{

}

CSVFileConfig::CSVFileConfig(const QString& filePath, const QString& browseDirectoryPath)
    : m_filePath{ filePath }
    , m_browseDirectoryPath{ browseDirectoryPath }
{

}

void CSVFileConfig::resetToDefault()
{
    m_filePath = "";
    m_browseDirectoryPath = QDir::currentPath();
}
