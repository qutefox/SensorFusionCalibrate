#include <QObject>
#include <QList>
#include <Eigen/Dense>

#include "calibrate_types.h"
#include "calibration_point.h"

class Calibrate : public QObject
{
    Q_OBJECT

public:
    Calibrate(QObject* parent = nullptr);

public slots:
    void addPoint(double x, double y, double z);
    void reset();
    void calibrate();

private:
    static const Eigen::Matrix<double, 6, 6> preInvertedConstraintMatrix;
    QList<CalibrationPoint> points;
};
