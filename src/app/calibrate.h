#pragma once

#include <QObject>
#include <QSet>
#include <QReadWriteLock>
#include <Eigen/Dense>

#include <set>

#include "../data/data_types.h"
#include "../data/point.h"
#include "../data/calibration_result.h"

class Calibrate : public QObject
{
    Q_OBJECT

public:
    Calibrate(QObject* parent = nullptr);

public slots:
    void addPoint(const Point& point);
    void addPoint(double x, double y, double z);
    void addPoints(const std::set<Point>& points);
    void reset();
    void calibrate();

    CalibrationResult getResult();

signals:
    void resultReady(CalibrationResult result);

private:
    static const Matrix6x6d preInvertedConstraintMatrix;
    std::set<Point> m_points;
    QReadWriteLock m_inputLock;
    QReadWriteLock m_outputLock;
    CalibrationResult m_result;

};
