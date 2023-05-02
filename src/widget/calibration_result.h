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
    CalibrationResultWidget(std::size_t deviceId, QWidget *parent = nullptr);
    virtual ~CalibrationResultWidget();

    void setWidgetPrecision(int precision);
    void setDeviceId(std::size_t deviceId);
    void update(const CalibrationResult& m_result);
    void clear();

signals:
    void resetCalibrationData();

private slots:
    void resetToolButtonClicked();

private:
    Ui::CalibrationResultWidget* m_ui = nullptr;
    std::ostringstream oss;
    int m_widgetPrecision = 4;
};
