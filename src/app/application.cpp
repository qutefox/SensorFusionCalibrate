#include <QHBoxLayout>

#include "application.h"

#include "../widget/ui_main_window.h"

#include <datasource/file/csv_file.h>
#include <datasource/serial/serialport.h>
#include <widget/calibration_result.h>

Application::Application(int argc, char *argv[])
    : QApplication{ argc, argv }
    , m_mainWindow{ new MainWindow() }
    , m_timer{ new QTimer(this) }
{
    QComboBox* dataSourceComboBox = m_mainWindow->ui()->dataSourceComboBox;

    dataSourceComboBox->addItem(CSVFileDataSource::getName(), "csv_file");
    dataSourceComboBox->addItem(SerialPortDataSource::getName(), "serial");

    connect(
        dataSourceComboBox, &QComboBox::currentTextChanged,
        this, &Application::dataSourceChanged
    );

    connect(
        m_timer, &QTimer::timeout,
        this, &Application::update
    );

    selectDataSource(m_settings.getDataSourceType());

    m_mainWindow->show();
}

Application::~Application()
{
    resetTabWidgets();
    removeDataSourceWidgets();
}

void Application::removeDataSourceWidgets()
{
    QWidget* dataSourceWidget = m_mainWindow->ui()->dataSourceFrame;
    QHBoxLayout* dataSourceLayout = qobject_cast<QHBoxLayout*>(dataSourceWidget->layout());
    if (m_dataSource)
    {
        if (m_dataSource->getTypeName() == "csv_file")
        {
            auto w = qobject_cast<const CSVFileControlWidget*>(m_dataSource->widget());
            m_settings.setCSVFileConfig(w->getValues());
        }
        else if (m_dataSource->getTypeName() == "serial")
        {
            auto w = qobject_cast<const SerialPortControlWidget*>(m_dataSource->widget());
            m_settings.setSerialPortConfig(w->getValues());
        }

        dataSourceLayout->removeWidget(m_dataSource->widget());
        m_dataSource.reset();
    }
}

QWidget* Application::getTabByName(const QString& name)
{
    QTabWidget* tabWidget = m_mainWindow->ui()->tabWidget;
    for (std::size_t i = 0; i < tabWidget->count(); ++i)
    {
        QWidget* tabElementWidget = tabWidget->widget(static_cast<int>(i));
        QString tabName = tabElementWidget->objectName();
        if (tabName == name)
        {
            return tabElementWidget;
        }
    }
    return nullptr;
}

void Application::addDataSourceWidget()
{
    QWidget* tabElementWidget = getTabByName("result");
    if (tabElementWidget == nullptr) return;

    unsigned int deviceId = static_cast<unsigned int>(m_calibrators.size())+1;
    CalibrationResultWidget* resultWidget = new CalibrationResultWidget(deviceId, tabElementWidget);
    m_calibrators.push_back(std::make_unique<Calibrate>(deviceId, resultWidget));
    tabElementWidget->layout()->addWidget(resultWidget);
}

void Application::resetResultWidgets()
{
    QWidget* tabElementWidget = getTabByName("result");
    if (tabElementWidget == nullptr) return;

    m_calibrators.clear();

    QLayoutItem* wItem;
    while ((wItem = tabElementWidget->layout()->takeAt(0)) != nullptr)
    {
        if (wItem->widget())
        {
            wItem->widget()->setParent(nullptr);
        }
        delete wItem;
    }    
}

void Application::resetTabWidgets()
{
    resetResultWidgets();

    QTabWidget* tabWidget = m_mainWindow->ui()->tabWidget;
    for (int i = 0; i < tabWidget->count(); i++)
    {
        QWidget* tabElementWidget = tabWidget->widget(i);
        QString tabName = tabElementWidget->objectName();
        if (tabName == "result") continue;
        tabWidget->removeTab(i);
    }
}

void Application::selectDataSource(const QString& value)
{
    QComboBox* dataSourceComboBox = m_mainWindow->ui()->dataSourceComboBox;
    int index = dataSourceComboBox->findData(value);
    if (index == -1) return;
    dataSourceComboBox->setCurrentIndex(index);
}

void Application::dataSourceChanged(QString dataSourceName)
{
    QWidget* dataSourceWidget = m_mainWindow->ui()->dataSourceFrame;
    QHBoxLayout* dataSourceLayout = qobject_cast<QHBoxLayout*>(dataSourceWidget->layout());

    removeDataSourceWidgets();
    resetTabWidgets();

    if (dataSourceName == CSVFileDataSource::getName())
    {
        m_dataSource = std::make_unique<CSVFileDataSource>(m_settings.getCSVFileConfig(), dataSourceWidget);
    }
    else if (dataSourceName == SerialPortDataSource::getName())
    {
        m_dataSource = std::make_unique<SerialPortDataSource>(m_settings.getSerialPortConfig(), dataSourceWidget);
    }

    if (m_dataSource)
    {
        dataSourceLayout->addWidget(m_dataSource->widget());
        m_dataSource->makeTabs(m_mainWindow->ui()->tabWidget);

        connect(
            m_dataSource.get(), &IDataSource::dataSourceStarted,
            this, &Application::dataSourceStarted
        );
        connect(
            m_dataSource.get(), &IDataSource::dataSourceEnded,
            this, &Application::dataSourceEnded
        );
        connect(
            m_dataSource.get(), &IDataSource::errorOccurred,
            this, &Application::dataSourceFailed
        );
        connect(
            m_dataSource.get(), &IDataSource::dataAvailable,
            this, &Application::dataSourceDataAvailable
        );

        QComboBox* dataSourceComboBox = m_mainWindow->ui()->dataSourceComboBox;
        m_settings.setDataSourceType(dataSourceComboBox->currentData().toString());
    }
}

void Application::dataSourceStarted()
{
    if (!m_dataSource) return;

    resetResultWidgets();

    if (m_dataSource->isStream() && !m_dataSource->canSignalDataAvailable())
    {
        // Start an update timer and periodically read the buffered stream data.
        m_timer->start(500);
    }

    update();
}

void Application::dataSourceEnded()
{
    m_timer->stop();
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
        addDataSourceWidget();
    }

    for (std::size_t i = 0 ; i < deviceCount ; ++i)
    {
        m_calibrators[i]->addPoints(devicePoints[i]); // Will update the user interface.
    }

}
