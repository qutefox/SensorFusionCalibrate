#include "calibrate.h"

#include <iostream>
#include <iomanip>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

const Eigen::Matrix<double, 6, 6> Calibrate::preInvertedConstraintMatrix
{
    {0.0, 0.5, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.0, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, -0.25, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, -0.25, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, -0.25},
};

Calibrate::Calibrate(unsigned int deviceId, CalibrationResultWidget* widget)
    : QObject(nullptr)
    , m_widget{ widget }
    , m_deviceId{ deviceId }
{
    m_widget->clear();

    connect(
        widget, &CalibrationResultWidget::resetCalibrationData,
        this, &Calibrate::reset
    );

    connect(
        widget, &CalibrationResultWidget::saveCalibrationData,
        this, &Calibrate::saveResultToFile
    );
}

void Calibrate::updateUserInterface()
{
    m_outputLock.lockForRead();
    m_widget->update(m_result);
    m_outputLock.unlock();
    emit userInterfaceUpdated();
}

void Calibrate::addPoint(const Point& point)
{
    bool changed = false;
    m_inputLock.lockForWrite();
    changed = m_points.insert(point).second;
    m_inputLock.unlock();
    if (changed) calibrate();
}

void Calibrate::addPoint(double x, double y, double z)
{
    bool changed = false;
    m_inputLock.lockForWrite();
    changed = m_points.insert({x, y, z}).second;
    m_inputLock.unlock();
    if (changed) calibrate();
}

void Calibrate::addPoints(const std::set<Point>& points)
{
    if (points.empty()) return;
    bool changed = false;
    m_inputLock.lockForWrite();
    std::size_t beforeSize = m_points.size();
    m_points.insert(points.begin(), points.end());
    std::size_t afterSize = m_points.size();
    if (afterSize > beforeSize) changed = true;
    m_inputLock.unlock();
    if (changed) calibrate();
}

void Calibrate::reset()
{
    m_inputLock.lockForWrite();
    m_outputLock.lockForWrite();

    m_points.clear();
    m_result.reset();
    m_widget->clear();

    m_inputLock.unlock();
    m_outputLock.unlock();
}

void Calibrate::saveResultToFile()
{
    CalibrationResult r;
    m_outputLock.lockForRead();
    r = m_result;
    m_outputLock.unlock();

    QString fileName = QString("%1_result.txt").arg(m_deviceId); // TODO: add folder path from (saved) settings.
    QString saveFileName = QFileDialog::getSaveFileName(m_widget,
        "Save File", fileName, "txt files (*.txt)");
    QFile file(saveFileName);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream fstream(&file);

        const Vector3d& bias = r.biasVector;
        fstream << "bias" << Qt::endl;
        fstream << bias[0] << "\t" << bias[1] << "\t" << bias[2] << Qt::endl;

        const Matrix3x3d& trMatrix = r.transformationMatrix;
        fstream << "transformationMatrix" << Qt::endl;
        fstream << trMatrix(0, 0) << "\t" << trMatrix(0, 1) << "\t" << trMatrix(0, 2) << Qt::endl;
        fstream << trMatrix(1, 0) << "\t" << trMatrix(1, 1) << "\t" << trMatrix(1, 2) << Qt::endl;
        fstream << trMatrix(2, 0) << "\t" << trMatrix(2, 1) << "\t" << trMatrix(2, 2) << Qt::endl;
    }
}

CalibrationResult Calibrate::getResult()
{
    CalibrationResult result;
    m_outputLock.lockForRead();
    result = m_result;
    m_outputLock.unlock();
    return result;
}

void Calibrate::calibrate()
{
    Eigen::Matrix<double, 10, Eigen::Dynamic> inputMatrix;

    m_inputLock.lockForRead();

    inputMatrix.resize(Eigen::NoChange, m_points.size());
    std::size_t i = 0;
    for (const Point& p : m_points)
    {
        std::cout << p << std::endl << std::flush;
        inputMatrix.col(i++) = p.toVect();
    }

    m_inputLock.unlock();

    Matrix10x10d symmetricInputMatrix = inputMatrix * inputMatrix.transpose();
    Matrix6x6d symmetric11   = symmetricInputMatrix.block(0, 0, 6, 6);
    Matrix6x4d symmetric12   = symmetricInputMatrix.block(0, 6, 6, 4);
    Matrix4x6d symmetric12t  = symmetricInputMatrix.block(6, 0, 4, 6);
    Matrix4x4d symmetric22   = symmetricInputMatrix.block(6, 6, 4, 4);
    Matrix4x4d symmetric22_1 = symmetric22.llt().solve(Matrix4x4d::Identity());
    Matrix4x6d symmetric22a = symmetric22_1 * symmetric12t;
    Matrix6x6d symmetric22b = symmetric12 * symmetric22a;
    Matrix6x6d ss = symmetric11 - symmetric22b;
    Matrix6x6d e = preInvertedConstraintMatrix * ss;

    Eigen::EigenSolver<Matrix6x6d> eigenSolver(e, true);
    Eigen::Index maxIndex;
    eigenSolver.eigenvalues().real().maxCoeff(&maxIndex);
    Eigen::VectorXcd eigenVector = eigenSolver.eigenvectors().col(maxIndex);
    Eigen::VectorXd v1 = eigenVector.real();
    Vector4d v2 = symmetric22a * v1;
    Vector10d v {v1[0], v1[1], v1[2], v1[3], v1[4], v1[5], -v2[0], -v2[1], -v2[2], -v2[3]};
    Matrix3x3d q {{v[0], v[5], v[4]}, {v[5], v[1], v[3]}, {v[4], v[3], v[2]}};
    Vector3d u {v[6], v[7], v[8]};
    Matrix3x3d q1 = q.llt().solve(Matrix3x3d::Identity());

    m_outputLock.lockForWrite();

    m_result.biasVector = q1 * u;
    m_result.biasVector *= -1;
    // b[0] -> x-axis combined bias
    // b[1] -> y-axis combined bias
    // b[2] -> z-axis combined bias
    Vector3d qb = q * m_result.biasVector;
    Vector1d btqb = m_result.biasVector.transpose() * qb;
    double hmb = std::sqrt(btqb(0,0)-v[9]);

    Eigen::EigenSolver<Matrix3x3d> eigenSolver2(q, true);
    Vector3d eigenReal = eigenSolver2.eigenvalues().real();

    Matrix3x3d dz = Eigen::MatrixXd::Zero(3,3);
    dz(0, 0) = std::sqrt(eigenReal[0]);
    dz(1, 1) = std::sqrt(eigenReal[1]);
    dz(2, 2) = std::sqrt(eigenReal[2]);

    Matrix3x3d vdz = eigenSolver2.eigenvectors().real() * dz;
    Matrix3x3d sq = vdz * eigenSolver2.eigenvectors().real().transpose();
    double hm = 0.569; // TODO: config?!
    m_result.transformationMatrix = sq * hm / hmb;

    m_outputLock.unlock();

    updateUserInterface();

    m_outputLock.lockForRead();
    emit resultReady(m_result);
    m_outputLock.unlock();
}
