#include "mainwindow.h"

#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow) {
    mUi->setupUi(this);

    connect(mUi->timeFactorSlider, &QSlider::valueChanged, mUi->glWidget->world(), &World::setTimeFactor);
    connect(mUi->timeFactorSlider, &QSlider::valueChanged, this, &MainWindow::onTimeFactorSliderChanged);
    mUi->timeFactorSlider->setValue(0);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    mLastPos = event->pos();
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QPoint offset = (event->pos() - mLastPos)/2.f;
//    mUi->glWidget->matrix().translate(offset.x(), offset.y());
//    mUi->glWidget->matrix().rotate(offset.manhattanLength()/10., offset.y(), -offset.x(), 0);
    float ex, ey, ez;
    mUi->glWidget->quanternion().getEulerAngles(&ex, &ey, &ez);
    if(ex+offset.y() > -90.f && ex+offset.y() < 1.f) {
        mUi->glWidget->quanternion() = QQuaternion::fromEulerAngles(ex+offset.y(), 0, 0);
    }
    qDebug() << ex << ey;
    mLastPos = event->pos();
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0) {
        mUi->glWidget->scale() *= 1.05;
//        mUi->glWidget->matrix().translate(0,0,-10);
    } else if(event->delta() < 0) {
        mUi->glWidget->scale() *= 0.95;
//        mUi->glWidget->matrix().translate(0,0,10);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Space:
            mUi->glWidget->setClearBuffer(!mUi->glWidget->isClearBufferEnabled());
            break;
        default:
            break;
    }
    event->accept();
}

void MainWindow::onTimeFactorSliderChanged(int value) {
    mUi->timeFactorLabel->setText(QString("x%1").arg(value));
}
