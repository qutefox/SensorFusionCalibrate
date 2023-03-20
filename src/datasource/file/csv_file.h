#pragma once

#include <set>
#include <memory>
#include <fstream>

#include <QString>
#include <QFile>
#include <QLineEdit>
#include <QToolButton>

#include <datasource/data_source_interface.h>
#include <widget/csv_file_control.h>

class CSVFileDataSource : public IDataSource
{
    Q_OBJECT

public:
    CSVFileDataSource(const QString& filePath, QObject* parent = nullptr);
    virtual ~CSVFileDataSource();

    static QString getName() { return "CSV File"; }
    bool getNextPoints(std::vector<std::set<Point>>& points) override;
    bool isStream() const override { return false; }
    QWidget* widget() const override;

private slots:
    void processFile(QString);

private:
    QFile m_csvFile;
    CSVFileControlWidget* m_widget;
};
