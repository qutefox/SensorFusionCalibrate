#pragma once

#include <set>
#include <memory>
#include <fstream>

#include <QString>
#include <QFile>
#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QRegularExpression>

#include "idatasource.h"

class CSVFileDataSource : public IDataSource
{
    Q_OBJECT

public:
    CSVFileDataSource(QWidget* parent = nullptr);
    virtual ~CSVFileDataSource();

    static QString getName() { return "CSV File"; }
    bool getNextPoints(std::vector<std::set<Point>>& points) override;
    bool isStream() const override { return false; }

private slots:
    void openFileDialog();
    void isFilePathValid(QString filePath);
    void processFile();

private:
    QLineEdit* m_lineEdit;
    QToolButton* m_toolButtonChoose;
    QToolButton* m_toolButtonProcess;
    QFile m_csvFile;
    std::set<Point> m_points;
    static const QRegularExpression numRegex;

    void processStarted();
    void processFailed();
    void processDone();
};
