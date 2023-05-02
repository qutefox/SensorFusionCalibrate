#pragma once

#include <memory>
#include <vector>

#include <QApplication>
#include <QTimer>

#include "calibrate.h"
#include "settings.h"

#include <widget/main_window.h>
#include <datasource/data_source_interface.h>

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char *argv[]);
    virtual ~Application();

private slots:
    void dataSourceChanged(QString dataSourceName);
    void dataSourceStarted();
    void dataSourceEnded();
    void dataSourceFailed(QString errorMessage);
    void dataSourceDataAvailable();
    void update();

private:
    MainWindow* m_mainWindow = nullptr;
    QTimer* m_timer = nullptr;
    std::unique_ptr<IDataSource> m_dataSource;
    Settings m_settings;

    std::vector<std::unique_ptr<Calibrate>> m_calibrators;

    void selectDataSource(const QString& value);
    void removeDataSourceWidgets();
    void resetTabWidgets();
    void resetResultWidgets();
    void addDataSourceWidget();

    QWidget* getTabByName(const QString& name);
};
