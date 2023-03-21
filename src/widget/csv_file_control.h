#pragma once

#include <QWidget>

#include <data/csv_file_config.h>

QT_BEGIN_NAMESPACE
namespace Ui { class CSVFileControlWidget; }
QT_END_NAMESPACE

class CSVFileControlWidget : public QWidget
{
    Q_OBJECT

public:
    CSVFileControlWidget(const CSVFileConfig& config, QWidget *parent = nullptr);
    virtual ~CSVFileControlWidget();

    void setFilePath(const QString& filePath);
    QString getFilePath() const;

    void setBrowseDirectoryPath(const QString& dirPath);
    QString getBrowseDirectoryPath() const;

    CSVFileConfig getValues() const;

signals:
    void filePathChanged(QString);
    void processFile(QString);

private slots:
    void filePathChangedSlot(QString);
    void openFileDialog();
    void processSlot();

private:
    Ui::CSVFileControlWidget* m_ui = nullptr;
    QString m_browseDirectoryPath;

    bool isFilePathValid(const QString& filePath);
};
