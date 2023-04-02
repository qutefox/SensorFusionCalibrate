#pragma once

#include <set>
#include <memory>
#include <fstream>

#include <QString>
#include <QFile>
#include <QLineEdit>
#include <QToolButton>

#include <datasource/data_source_interface.h>
#include <datasource/file/csv_file_config.h>
#include <widget/csv_file_control.h>

class CSVFileDataSource : public IDataSource
{
    Q_OBJECT

public:
    CSVFileDataSource(const CSVFileConfig& config, QObject* parent = nullptr);
    virtual ~CSVFileDataSource();

    static QString getName() { return "CSV File"; }
    bool getNextPoints(std::vector<std::set<Point>>& points) override;
    bool isStream() const override { return false; }
    bool canSignalDataAvailable() const override { return false; }
    QString getTypeName() const override { return "csv_file"; }
    QWidget* widget() const override;

private slots:
    void processFile(QString);

private:
    QFile m_csvFile;
    CSVFileControlWidget* m_widget;
};
