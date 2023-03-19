#include <QHBoxLayout>

#include "application.h"
#include "../widget/ui_mainwindow.h"
#include "../datasource/csv_file.h"
#include "../datasource/serial_port.h"

Application::Application(int argc, char *argv[])
    : QApplication{ argc, argv }
    , m_mainWindow{ new MainWindow() }
    , m_timer{ new QTimer(this) }
{

    m_mainWindow->ui()->dataSourceComboBox->addItems(
        {
            CSVFileDataSource::getName(),
            SerialPortDataSource::getName()
        }
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
    cleanup();
}

void Application::cleanup()
{
    QWidget* tabWidget = m_mainWindow->ui()->tabWidget->widget(0);
    QVBoxLayout* tabLayout = qobject_cast<QVBoxLayout*>(tabWidget->layout());

    for (auto& c : m_calibrators)
    {
        tabLayout->removeWidget(c.get());
        c->reset();
    }

    m_calibrators.clear();
}

void Application::dataSourceChanged(QString dataSourceName)
{
    QWidget* dataSourceWidget = m_mainWindow->ui()->dataSourceFrame;
    QHBoxLayout* dataSourceLayout = qobject_cast<QHBoxLayout*>(dataSourceWidget->layout());

    if (m_dataSource)
    {
        dataSourceLayout->removeWidget(m_dataSource.get());
    }

    m_dataSource.reset();

    if (dataSourceName == CSVFileDataSource::getName())
    {
        m_dataSource = std::make_unique<CSVFileDataSource>(dataSourceWidget);
    }
    else if (dataSourceName == SerialPortDataSource::getName())
    {
        m_dataSource = std::make_unique<SerialPortDataSource>(dataSourceWidget);
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
            m_dataSource.get(), SIGNAL(dataAvailable()),
            this, SLOT(dataSourceDataAvailable())
        );
    }
}

void Application::dataSourceOpened()
{
    if (!m_dataSource) return;
    if (!m_dataSource->isStream()) m_timer->start(500);
    else cleanup();
}

void Application::dataSourceClosed()
{
    if (!m_dataSource) return;
    if (!m_dataSource->isStream()) m_timer->stop();
}

void Application::dataSourceFailed(QString errorMessage)
{
    // Show error message for 10 seconds.
    m_mainWindow->ui()->statusbar->showMessage(QString("Error: %1").arg(errorMessage), 10000);
}

void Application::dataSourceDataAvailable()
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

        QWidget* tabWidget = m_mainWindow->ui()->tabWidget->widget(0);
        QVBoxLayout* tabLayout = qobject_cast<QVBoxLayout*>(tabWidget->layout());

        m_calibrators.push_back(std::make_unique<Calibrate>(m_calibrators.size()+1, tabWidget));
        QWidget* calibratorWidget = m_calibrators.back().get();
        tabLayout->addWidget(calibratorWidget);
    }

    for (std::size_t i = 0 ; i < deviceCount ; ++i)
    {
        m_calibrators[i]->addPoints(devicePoints[i]); // Will update the user interface.
    }

}
