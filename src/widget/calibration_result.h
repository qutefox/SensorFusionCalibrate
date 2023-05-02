#pragma once

#include <sstream>

#include <QGroupBox>
#include <QWidget>
#include <QString>

#include <data/calibration_result.h>

QT_BEGIN_NAMESPACE
namespace Ui { class CalibrationResultWidget; }
QT_END_NAMESPACE

class CalibrationResultWidget : public QGroupBox
{
    Q_OBJECT

    QString double2QStr(double val);

public:
    CalibrationResultWidget(unsigned int deviceId, QWidget *parent = nullptr);
    virtual ~CalibrationResultWidget();

    void setWidgetPrecision(int precision);
    void setDeviceId(unsigned int deviceId);
    void update(const CalibrationResult& m_result);
    void clear();

signals:
    void resetCalibrationData();
    void saveCalibrationData();

private slots:
    void resetToolButtonClicked();
    void saveToolButtonClicked();

private:
    Ui::CalibrationResultWidget* m_ui = nullptr;
    std::ostringstream oss;
    int m_widgetPrecision = 4;
};
