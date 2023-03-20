#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CSVFileControlWidget; }
QT_END_NAMESPACE

class CSVFileControlWidget : public QWidget
{
    Q_OBJECT

public:
    CSVFileControlWidget(const QString& filePath, QWidget *parent = nullptr);
    virtual ~CSVFileControlWidget();

    void setFilePath(const QString& filePath);
    QString getFilePath() const;

signals:
    void filePathChanged(QString);
    void processFile(QString);

private slots:
    void filePathChangedSlot(QString);
    void openFileDialog();
    void processSlot();

private:
    Ui::CSVFileControlWidget* m_ui = nullptr;

    bool isFilePathValid(const QString& filePath);
};
