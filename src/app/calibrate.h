#pragma once

#include <set>

#include <QWidget>
#include <QSet>
#include <QReadWriteLock>
#include <QLabel>
#include <QLineEdit>
#include <Eigen/Dense>

#include "../data/data_types.h"
#include "../data/point.h"
#include "../data/calibration_result.h"

class Calibrate : public QWidget
{
    Q_OBJECT

public:
    Calibrate(int deviceId, QWidget* parent = nullptr);

    void setWidgetPrecision(int precision);

public slots:
    void addPoint(const Point& point);
    void addPoint(double x, double y, double z);
    void addPoints(const std::set<Point>& points);
    void reset();
    void calibrate();
    void updateUserInterface();

    CalibrationResult getResult();

signals:
    void resultReady(CalibrationResult result);
    void userInterfaceUpdated();

private:
    static const Matrix6x6d preInvertedConstraintMatrix;
    std::set<Point> m_points;
    QReadWriteLock m_inputLock;
    QReadWriteLock m_outputLock;
    CalibrationResult m_result;

    QLabel* m_biasLabel;
    QLineEdit* m_biasLineEditX;
    QLineEdit* m_biasLineEditY;
    QLineEdit* m_biasLineEditZ;

    QLabel* m_transformationMatrixLabel;
    QLineEdit* m_transformationMatrixLineEdit11;
    QLineEdit* m_transformationMatrixLineEdit12;
    QLineEdit* m_transformationMatrixLineEdit13;
    QLineEdit* m_transformationMatrixLineEdit21;
    QLineEdit* m_transformationMatrixLineEdit22;
    QLineEdit* m_transformationMatrixLineEdit23;
    QLineEdit* m_transformationMatrixLineEdit31;
    QLineEdit* m_transformationMatrixLineEdit32;
    QLineEdit* m_transformationMatrixLineEdit33;

    std::ostringstream oss;
    int m_widgetPrecision = 4;
    int m_deviceId;

    QString double2QStr(double val);

};
