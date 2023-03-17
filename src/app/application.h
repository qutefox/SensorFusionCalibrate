#pragma once

#include <memory>

#include <QApplication>

#include "calibrate.h"
#include "settings.h"

#include "../datasource/idatasource.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char *argv[]);
    virtual ~Application();

private:
    Settings m_settings;
    Calibrate m_calibrate;
    std::unique_ptr<IDataSource> m_dataSource;
};
