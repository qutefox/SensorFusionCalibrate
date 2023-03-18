#pragma once

#include <sstream>

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>

#include "../data/calibration_result.h"

class CalibrationResultWidget : QWidget
{
    Q_OBJECT

public:
    CalibrationResultWidget(int precision, QWidget* parent = nullptr);

    void setPrecision(int precision);

public slots:
    void updateValues(const CalibrationResult& result);

signals:
    void updateDone();

private:
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
    int m_precision = 4;

    QString double2QStr(double val);
};
