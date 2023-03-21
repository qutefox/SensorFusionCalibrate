#include "csv_file_control.h"
#include "./ui_csv_file_control.h"

#include <QFileDialog>

CSVFileControlWidget::CSVFileControlWidget(const CSVFileConfig& config, QWidget *parent)
    : QWidget(parent)
    , m_ui{ new Ui::CSVFileControlWidget }
{
    m_ui->setupUi(this);

    connect(
        m_ui->filePathLineEdit, &QLineEdit::textChanged,
        this, &CSVFileControlWidget::filePathChangedSlot
    );

    connect(
        m_ui->browseToolButton, &QToolButton::released,
        this, &CSVFileControlWidget::openFileDialog
    );

    connect(
        m_ui->processToolButton, &QToolButton::released,
        this, &CSVFileControlWidget::processSlot
    );

    setFilePath(config.m_filePath);
    setBrowseDirectoryPath(config.m_browseDirectoryPath);
}

CSVFileControlWidget::~CSVFileControlWidget()
{
    delete m_ui;
}

void CSVFileControlWidget::setFilePath(const QString& filePath)
{
    m_ui->filePathLineEdit->setText(filePath);
}

QString CSVFileControlWidget::getFilePath() const
{
    return m_ui->filePathLineEdit->text();
}

void CSVFileControlWidget::setBrowseDirectoryPath(const QString& dirPath)
{
    QFileInfo checkDir(dirPath);
    if (checkDir.exists()  && checkDir.isDir())
    {
        m_browseDirectoryPath = dirPath;
    }
}

QString CSVFileControlWidget::getBrowseDirectoryPath() const
{
    return m_browseDirectoryPath;
}

CSVFileConfig CSVFileControlWidget::getValues() const
{
    return CSVFileConfig(
        getFilePath(),
        getBrowseDirectoryPath()
    );
}

void CSVFileControlWidget::filePathChangedSlot(QString newPath)
{
    if (isFilePathValid(newPath))
    {
        m_ui->processToolButton->setText("Process");
        m_ui->processToolButton->setStyleSheet("QToolButton{color:green;}");
        m_ui->processToolButton->setEnabled(true);
    }
    else
    {
        m_ui->processToolButton->setText("Invalid file path");
        m_ui->processToolButton->setStyleSheet("QToolButton{color:red;}");
        m_ui->processToolButton->setEnabled(false);
    }
}

void CSVFileControlWidget::openFileDialog()
{
    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");

    QString dirPath = getBrowseDirectoryPath();
    QFileInfo checkDir(dirPath);
    if (!checkDir.exists() || !checkDir.isDir())
    {
        dirPath = QDir::currentPath();
    }

    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open CSV File",
        dirPath,
        filters, &defaultFilter
    );

    if (filePath.isEmpty()) return;

    setFilePath(filePath);

    QDir dir = QFileInfo(filePath).absoluteDir();
    setBrowseDirectoryPath(dir.absolutePath());
}

void CSVFileControlWidget::processSlot()
{
    emit processFile(getFilePath());
}

bool CSVFileControlWidget::isFilePathValid(const QString& filePath)
{
    QFileInfo checkFile(filePath);
    return checkFile.exists() && checkFile.isFile();
}
