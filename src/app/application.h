#pragma once

#include <memory>
#include <vector>

#include <QApplication>
#include <QTimer>

#include "calibrate.h"
#include "settings.h"

#include "../widget/mainwindow.h"
#include "../datasource/idatasource.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char *argv[]);
    virtual ~Application();

private:
    MainWindow* m_mainWindow = nullptr;
    QTimer* m_timer = nullptr;
    std::unique_ptr<IDataSource> m_dataSource;
    Settings m_settings;

    std::vector<std::unique_ptr<Calibrate>> m_calibrators;

public slots:
    void cleanup();

private slots:
    void dataSourceChanged(QString dataSourceName);
    void dataSourceOpened();
    void dataSourceClosed();
    void dataSourceFailed(QString errorMessage);
    void dataSourceDataAvailable();
    void update();
};
