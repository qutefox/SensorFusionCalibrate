#include "calibration_result_widget.h"

#include <iomanip>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

CalibrationResultWidget::CalibrationResultWidget(int precision, QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* biasLayout = new QHBoxLayout();

    m_biasLineEditX = new QLineEdit();
    m_biasLineEditY = new QLineEdit();
    m_biasLineEditZ = new QLineEdit();

    m_biasLineEditX->setReadOnly(true);
    m_biasLineEditY->setReadOnly(true);
    m_biasLineEditZ->setReadOnly(true);

    biasLayout->addWidget(m_biasLineEditX);
    biasLayout->addWidget(m_biasLineEditY);
    biasLayout->addWidget(m_biasLineEditZ);

    QGridLayout* gridLayout = new QGridLayout();

    m_transformationMatrixLineEdit11 = new QLineEdit();
    m_transformationMatrixLineEdit12 = new QLineEdit();
    m_transformationMatrixLineEdit13 = new QLineEdit();

    m_transformationMatrixLineEdit11->setReadOnly(true);
    m_transformationMatrixLineEdit12->setReadOnly(true);
    m_transformationMatrixLineEdit13->setReadOnly(true);

    gridLayout->addWidget(m_transformationMatrixLineEdit11, 1, 1);
    gridLayout->addWidget(m_transformationMatrixLineEdit12, 1, 2);
    gridLayout->addWidget(m_transformationMatrixLineEdit13, 1, 3);

    m_transformationMatrixLineEdit21 = new QLineEdit();
    m_transformationMatrixLineEdit22 = new QLineEdit();
    m_transformationMatrixLineEdit23 = new QLineEdit();

    m_transformationMatrixLineEdit21->setReadOnly(true);
    m_transformationMatrixLineEdit22->setReadOnly(true);
    m_transformationMatrixLineEdit23->setReadOnly(true);

    gridLayout->addWidget(m_transformationMatrixLineEdit21, 2, 1);
    gridLayout->addWidget(m_transformationMatrixLineEdit22, 2, 2);
    gridLayout->addWidget(m_transformationMatrixLineEdit23, 2, 3);

    m_transformationMatrixLineEdit31 = new QLineEdit();
    m_transformationMatrixLineEdit32 = new QLineEdit();
    m_transformationMatrixLineEdit33 = new QLineEdit();

    m_transformationMatrixLineEdit31->setReadOnly(true);
    m_transformationMatrixLineEdit32->setReadOnly(true);
    m_transformationMatrixLineEdit33->setReadOnly(true);

    gridLayout->addWidget(m_transformationMatrixLineEdit31, 3, 1);
    gridLayout->addWidget(m_transformationMatrixLineEdit32, 3, 2);
    gridLayout->addWidget(m_transformationMatrixLineEdit33, 3, 3);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    m_biasLabel = new QLabel("Bias");
    m_transformationMatrixLabel = new QLabel("Transformation Matrix");

    mainLayout->addWidget(m_biasLabel);
    mainLayout->addLayout(biasLayout);
    mainLayout->addWidget(m_transformationMatrixLabel);
    mainLayout->addLayout(gridLayout);

    setPrecision(precision);
}

void CalibrationResultWidget::setPrecision(int precision)
{
    if (precision > 0) m_precision = precision;
}

QString CalibrationResultWidget::double2QStr(double val)
{
    oss.str("");
    oss << std::setprecision(m_precision) << val;
    return QString::fromStdString(oss.str());
}

void CalibrationResultWidget::updateValues(const CalibrationResult& result)
{
    m_biasLineEditX->setText(double2QStr(result.biasVector[0]));
    m_biasLineEditY->setText(double2QStr(result.biasVector[1]));
    m_biasLineEditZ->setText(double2QStr(result.biasVector[2]));

    m_transformationMatrixLineEdit11->setText(double2QStr(result.transformationMatrix(0, 0)));
    m_transformationMatrixLineEdit12->setText(double2QStr(result.transformationMatrix(0, 1)));
    m_transformationMatrixLineEdit13->setText(double2QStr(result.transformationMatrix(0, 2)));

    m_transformationMatrixLineEdit21->setText(double2QStr(result.transformationMatrix(1, 0)));
    m_transformationMatrixLineEdit22->setText(double2QStr(result.transformationMatrix(1, 1)));
    m_transformationMatrixLineEdit23->setText(double2QStr(result.transformationMatrix(1, 2)));

    m_transformationMatrixLineEdit31->setText(double2QStr(result.transformationMatrix(2, 0)));
    m_transformationMatrixLineEdit32->setText(double2QStr(result.transformationMatrix(2, 1)));
    m_transformationMatrixLineEdit33->setText(double2QStr(result.transformationMatrix(2, 2)));

    emit updateDone();
}

