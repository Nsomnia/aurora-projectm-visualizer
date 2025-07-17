#include "QtOpenGLWidget.h"
#include "utils/Logger.h"
#include <QTimer>
#include <projectM-4/projectM.h>

QtOpenGLWidget::QtOpenGLWidget(Core& core, QWidget* parent)
    : QOpenGLWidget(parent)
    , _core(core)
{
    int timerInterval = 16; // ~60 FPS
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&QtOpenGLWidget::update));
    timer->start(timerInterval);
}

QtOpenGLWidget::~QtOpenGLWidget()
{
    makeCurrent();
    _core.cleanup();
    doneCurrent();
}

void QtOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    _core.init(this);
    projectm_set_window_size(_core.projectM(), width(), height());
}

void QtOpenGLWidget::resizeGL(int w, int h)
{
    _core.resize(w, h);
    projectm_set_window_size(_core.projectM(), w, h);
}

void QtOpenGLWidget::paintGL()
{
    Logger::info("QtOpenGLWidget::paintGL() called");
    _core.render();
}
