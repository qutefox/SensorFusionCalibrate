#pragma once

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

    Ui::MainWindow* ui();
    QLabel* statusLabel();

private:
    Ui::MainWindow* m_ui;

    // status bar stuff is not handled by the QtDesigner properly. Have to do programmatically:
    QLabel* m_statusLabel;
};

