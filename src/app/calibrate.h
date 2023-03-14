#include <QObject>

#include <Eigen/Dense>

typedef Eigen::Matrix<double, 10, 1> Vector10d;
typedef Eigen::Matrix<double, 10, Eigen::Dynamic> Matrix10d;

class Calibrate : public QObject
{
    Q_OBJECT

public:
    Calibrate(QObject* parent = nullptr);

public slots:
    void AddPoint(double x, double y, double z);

private:
    Vector10d xyzToVect10d(double x, double y, double z);

    static const Eigen::Matrix<double, 6, 6> preInvertedConstraintMatrix;
    Matrix10d input;
    Eigen::Matrix<double, 10, 10> s;
    Eigen::Matrix<double, 6, 6> s11;
    Eigen::Matrix<double, 6, 4> s12;
    Eigen::Matrix<double, 4, 6> s12t;
    Eigen::Matrix<double, 4, 4> s22;
    Eigen::Matrix<double, 4, 4> s22_1;
    Eigen::Matrix<double, 4, 6> s22a;
    Eigen::Matrix<double, 6, 6> s22b;
    Eigen::Matrix<double, 6, 6> ss;
    Eigen::Matrix<double, 6, 6> e;
    Eigen::Matrix<double, 6, 6> sss;
    Eigen::Matrix<double, 6, 1> eigen_real;
    Eigen::Matrix<double, 6, 1> eigen_image;
    Eigen::Matrix<double, 6, 1> v1;
    Eigen::Matrix<double, 4, 1> v2;
    Eigen::Matrix<double, 10, 1> v;
    Eigen::Matrix<double, 9, 1> q;
    Eigen::Matrix<double, 3, 1> u;
    Eigen::Matrix<double, 9, 1> q_1;
    Eigen::Matrix<double, 3, 1> b;
    Eigen::Matrix<double, 3, 1> qb;
    Eigen::Matrix<double, 9, 1> ssss;
    Eigen::Matrix<double, 3, 1> eigen_real3;
    Eigen::Matrix<double, 3, 1> eigen_imag3;
    Eigen::Matrix<double, 9, 1> dz;
    Eigen::Matrix<double, 9, 1> vdz;
    Eigen::Matrix<double, 9, 1> sq;
    Eigen::Matrix<double, 9, 1> a_1;
};
