#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <data/quaternion.h>

class VisualizeWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    VisualizeWidget(QWidget *parent = nullptr);
    virtual ~VisualizeWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Quaternion m_orientation;
};
