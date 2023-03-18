#include <QHBoxLayout>

#include "application.h"
#include "../widget/ui_mainwindow.h"
#include "../datasource/csv_file.h"
// #include "../datasource/serial_port.h"

Application::Application(int argc, char *argv[])
    : QApplication{ argc, argv }
    , m_mainWindow{ new MainWindow() }
    , m_timer{ new QTimer(this) }
{

    m_mainWindow->ui()->dataSourceComboBox->addItems(
        {CSVFileDataSource::getName()}
    );

    connect(
        m_mainWindow->ui()->dataSourceComboBox, SIGNAL(currentTextChanged(QString)),
        this, SLOT(dataSourceChanged(QString))
    );

    connect(
        m_timer, SIGNAL(timeout()),
        this, SLOT(update())
    );

    m_mainWindow->show();
}

Application::~Application()
{

}

void Application::dataSourceChanged(QString dataSourceName)
{
    QHBoxLayout* dataSourceLayout = qobject_cast<QHBoxLayout*>(m_mainWindow->ui()->dataSourceFrame->layout());

    if (m_dataSource)
    {
        dataSourceLayout->removeWidget(m_dataSource.get());
    }

    m_dataSource.reset();
    if (dataSourceName == CSVFileDataSource::getName())
    {
        m_dataSource = std::make_unique<CSVFileDataSource>();
    }

    if (m_dataSource)
    {
        dataSourceLayout->addWidget(m_dataSource.get());
        connect(
            m_dataSource.get(), SIGNAL(opened()),
            this, SLOT(dataSourceOpened())
        );
        connect(
            m_dataSource.get(), SIGNAL(closed()),
            this, SLOT(dataSourceClosed())
        );
        connect(
            m_dataSource.get(), SIGNAL(failed(QString)),
            this, SLOT(dataSourceFailed(QString))
        );
        connect(
            m_dataSource.get(), SIGNAL(readNewPoints(unsigned long)),
            this, SLOT(dataSourceReadNewPoints(unsigned long))
        );
    }
}

void Application::dataSourceOpened()
{
    if (!m_dataSource) return;
    if (!m_dataSource->isStream()) m_timer->start(500);
}

void Application::dataSourceClosed()
{
    if (!m_dataSource) return;
    if (!m_dataSource->isStream()) m_timer->stop();
}

void Application::dataSourceFailed(QString errorMessage)
{
    // TODO: set statusbar error message
}

void Application::dataSourceReadNewPoints(unsigned long count)
{
    update();
}

void Application::update()
{
    if (!m_dataSource) return;
    std::vector<std::set<Point>> devicePoints;
    if(!m_dataSource->getNextPoints(devicePoints)) return;

    std::size_t deviceCount = devicePoints.size();
    while(deviceCount > m_calibrators.size())
    {
        // Need to add more calibrator instance.

        m_calibrators.push_back(std::make_unique<Calibrate>());
    }

    for (std::size_t i = 0 ; i < deviceCount ; ++i)
    {
        m_calibrators[i]->addPoints(devicePoints[i]);
    }

}
