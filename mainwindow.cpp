#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVariant>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    m_statusLabel = new QLabel(this);
    m_ui->statusBar->addWidget(m_statusLabel);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

Ui::MainWindow* MainWindow::ui()
{
    return m_ui;
}

QLabel* MainWindow::statusLabel()
{
    return m_statusLabel;
}

/*
void MainWindow::appendSerialTerminal(const QByteArray& in)
{
    ui->terminalPlainTextEdit->insertPlainText(in);
}

void MainWindow::setTerminalStatusText(const QString& status)
{
    ui->terminalLabel->setText(status);
}
*/
