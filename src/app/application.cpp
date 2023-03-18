#include <QHBoxLayout>

#include "application.h"
#include "../widget/ui_mainwindow.h"
#include "../datasource/csv_file.h"
#include "../datasource/serial_port.h"

Application::Application(int argc, char *argv[])
    : QApplication{ argc, argv }
    , m_mainWindow{ new MainWindow() }
{

    m_mainWindow->ui()->dataSourceComboBox->addItems(
        {CSVFileDataSource::getName()}
    );

    m_dataSource = new CSVFileDataSource();

    QHBoxLayout* dataSourceLayout = qobject_cast<QHBoxLayout*>(m_mainWindow->ui()->dataSourceFrame->layout());
    // dataSourceLayout.re
    dataSourceLayout->addWidget(m_dataSource);
    //dataSourceLayout);

    m_mainWindow->show();
}

Application::~Application()
{

}
