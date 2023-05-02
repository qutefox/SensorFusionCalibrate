#include "calibration_result.h"
#include "./ui_calibration_result.h"

#include <iomanip>

CalibrationResultWidget::CalibrationResultWidget(std::size_t deviceId, QWidget *parent)
    : QGroupBox(parent)
    , m_ui{ new Ui::CalibrationResultWidget }
{
    m_ui->setupUi(this);

    connect(
        m_ui->resetToolButton, &QToolButton::clicked,
        this, &CalibrationResultWidget::resetToolButtonClicked
    );

    setWidgetPrecision(8);
    setDeviceId(deviceId);
}

CalibrationResultWidget::~CalibrationResultWidget()
{
    delete m_ui;
}

void CalibrationResultWidget::resetToolButtonClicked()
{
    emit resetCalibrationData();
}

void CalibrationResultWidget::setDeviceId(std::size_t deviceId)
{
    this->setTitle("Device " + QString::number(deviceId));
}

void CalibrationResultWidget::setWidgetPrecision(int precision)
{
    if (precision > 0)
    {
        m_widgetPrecision = precision;
    }
}

QString CalibrationResultWidget::double2QStr(double val)
{
    oss.str("");
    oss << std::setprecision(m_widgetPrecision) << val;
    return QString::fromStdString(oss.str());
}

void CalibrationResultWidget::update(const CalibrationResult& m_result)
{
    const Vector3d& bias = m_result.biasVector;
    const Matrix3x3d& trMatrix = m_result.transformationMatrix;

    m_ui->biasLineEditX->setText(double2QStr(bias[0]));
    m_ui->biasLineEditY->setText(double2QStr(bias[1]));
    m_ui->biasLineEditZ->setText(double2QStr(bias[2]));

    m_ui->transformationMatrixLineEdit11->setText(double2QStr(trMatrix(0, 0)));
    m_ui->transformationMatrixLineEdit12->setText(double2QStr(trMatrix(0, 1)));
    m_ui->transformationMatrixLineEdit13->setText(double2QStr(trMatrix(0, 2)));

    m_ui->transformationMatrixLineEdit21->setText(double2QStr(trMatrix(1, 0)));
    m_ui->transformationMatrixLineEdit22->setText(double2QStr(trMatrix(1, 1)));
    m_ui->transformationMatrixLineEdit23->setText(double2QStr(trMatrix(1, 2)));

    m_ui->transformationMatrixLineEdit31->setText(double2QStr(trMatrix(2, 0)));
    m_ui->transformationMatrixLineEdit32->setText(double2QStr(trMatrix(2, 1)));
    m_ui->transformationMatrixLineEdit33->setText(double2QStr(trMatrix(2, 2)));
}

void CalibrationResultWidget::clear()
{
    m_ui->biasLineEditX->setText("");
    m_ui->biasLineEditY->setText("");
    m_ui->biasLineEditZ->setText("");

    m_ui->transformationMatrixLineEdit11->setText("");
    m_ui->transformationMatrixLineEdit12->setText("");
    m_ui->transformationMatrixLineEdit13->setText("");

    m_ui->transformationMatrixLineEdit21->setText("");
    m_ui->transformationMatrixLineEdit22->setText("");
    m_ui->transformationMatrixLineEdit23->setText("");

    m_ui->transformationMatrixLineEdit31->setText("");
    m_ui->transformationMatrixLineEdit32->setText("");
    m_ui->transformationMatrixLineEdit33->setText("");
}
