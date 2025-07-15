#include "QtOpenGLWidget.h"
#include <QPainter>

QtOpenGLWidget::QtOpenGLWidget(Core& core, QWidget* parent)
    : QWidget(parent), _core(core) {
}

QtOpenGLWidget::~QtOpenGLWidget() {}

void QtOpenGLWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::red);
    update();
}

void QtOpenGLWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
}
