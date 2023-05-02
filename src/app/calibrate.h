#pragma once

#include <set>

#include <QObject>
#include <QReadWriteLock>
#include <Eigen/Dense>

#include <data/data_types.h>
#include <data/point.h>
#include <data/calibration_result.h>

#include <widget/calibration_result.h>

// https://sites.google.com/view/sailboatinstruments1/a-download-magneto-v1-2
// https://sites.google.com/view/sailboatinstruments1/b-presentation
// https://www.mathworks.com/matlabcentral/fileexchange/23377-ellipsoid-fitting

/*
 *  Looks for the ellipsoid that best fits the magnetometer raw measurements.
 *  Uses a technique known as "Li's ellipsoid specific fitting algorithm".
 *  After the characteristics of the ellipsoid are established, a correction matrix can be developed.
 */

class Calibrate : public QObject
{
    Q_OBJECT

public:
    Calibrate(CalibrationResultWidget* widget);

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
    CalibrationResultWidget* m_widget;
};
