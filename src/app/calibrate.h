#pragma once

#include <set>

#include <QObject>
#include <QReadWriteLock>
#include <Eigen/Dense>

#include "../data/data_types.h"
#include "../data/point.h"

class Calibrate : public QObject
{
    Q_OBJECT

public:
    Calibrate(QObject* parent = nullptr);

public slots:
    void addPoint(const Point& point);
    void addPoint(double x, double y, double z);
    void reset();
    void calibrate();

    Vector3d getBias();
    Matrix3x3d getTensor();

private:
    static const Matrix6x6d preInvertedConstraintMatrix;
    std::set<Point> m_points;
    QReadWriteLock m_inputLock;
    QReadWriteLock m_outputLock;
    Vector3d m_bias;
    Matrix3x3d m_tensor;

};
