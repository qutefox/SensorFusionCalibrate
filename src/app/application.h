#pragma once

#include <memory>

#include <QApplication>

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
    IDataSource* m_dataSource = nullptr;

    Settings m_settings;
    Calibrate m_calibrate;

};
