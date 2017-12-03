#include "glwidget.h"

#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QMenu>
#include <QLabel>
#include <GL/glu.h>
#include "object.h"

GlWidget::GlWidget(QWidget *parent)
    : QGLWidget(parent),
      mWorld(new World) {
    connect(mWorld.data(), &World::recalced, this, &GlWidget::onWorldRecalced);
    connect(mWorld.data(), &World::recalced, this, &GlWidget::updateInfoLabel);
    connect(this, &GlWidget::customContextMenuRequested, this, &GlWidget::onCustomContextMenuRequested);

    setContextMenuPolicy(Qt::CustomContextMenu);

    mInfoLabel = new QLabel("", this);
    mInfoLabel->setGeometry(0,0,100,50);
    mInfoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mInfoLabel->setStyleSheet("color: rgb(200,200,200);"
                              "background-color: rgb(0,0,0);"
                              "padding: 5px;");

    QTimer *fpsZeroizeTimer = new QTimer(this);
    connect(fpsZeroizeTimer, &QTimer::timeout, this, &GlWidget::updateInfoLabel);
    connect(fpsZeroizeTimer, &QTimer::timeout, this, [=]() -> void { mFps = mFpsCounter; mFpsCounter = 0; });
    fpsZeroizeTimer->start(1000);
}

void GlWidget::initializeGL() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glPointSize(6.f);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
}

void GlWidget::resizeGL(int w, int h) {
    qDebug() << "Resize to" << w << h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2., w/2., h/2., -h/2., qMax(w,h)/2., -qMax(w,h)/2.);
//    gluPerspective(160., 1, 0.1, 1000);
//    gluLookAt(0,0,0.1, 0,0,300, 0,1,0);
    glMatrixMode(GL_MODELVIEW);

    if(mWorld->objects().isEmpty()) {
//        for(int i = 0; i < 50; ++i) {
//            double randomX = (double)qrand()/(double)RAND_MAX;
//            double randomY = (double)qrand()/(double)RAND_MAX;
//            double randomZ = (double)qrand()/(double)RAND_MAX;
//            mWorld->createObject(10000000000000, w*randomX - w/2., h*randomY - h/2., qMax(w,h)*randomZ - qMax(w,h)/2., 0, 0, 0);
//        }
        mWorld->createObject(2000000000000000, 0, 0, 10, 0, 0, 0);
        mWorld->createObject(10000000000000, -200, -70, 10, 3, 20, 2);
        mWorld->createObject(10000000000000, 200, -70, 10, 3, -20, 2);
        mWorld->createObject(10000000000000, -180, -10, 10, 3, 20, 2);
        mWorld->createObject(10000000000000, 120, 70, 10, 20, -20, 2);

//        mWorld->createObject(1000000000000000, 0, 500, 0, 0, 0, 0);
    }
}

void GlWidget::paintGL() {
    if(mClearBuffer) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPointSize(6.f);
    } else {
        glPointSize(1.f);
    }

    glMatrixMode(GL_MODELVIEW);
    QMatrix4x4 transformation(mMatrix);
    transformation.scale(mScale);
    transformation.rotate(mQuanternion);
    glLoadMatrixf(transformation.data());


    glColor4f(0.8f, 4.f, 4.f, 0.7f);
    glBegin(GL_POINTS);
    for(Object *object : mWorld->objects()) {
        glVertex3f(object->pos().x(), object->pos().y(), object->pos().z());
    }
    glEnd();

    glLineWidth(1.);
    glColor4f(2.5f, 0.f, 0.f, 0.2f);
    glBegin(GL_LINES);
    for(Object *object : mWorld->objects()) {
        glVertex3f(object->pos().x(), object->pos().y(), object->pos().z());
        glVertex3f(object->worldAccel().x(), object->worldAccel().y(), object->worldAccel().z());
    }
    glEnd();

    glColor4f(0.5f, 2.f, 2.f, 0.05f);
    glBegin(GL_LINES);
    for(Object *object : mWorld->objects()) {
        for(Object *subobject : mWorld->objects()) {
            if(subobject->id() != object->id()) {
                glVertex3f(object->pos().x(), object->pos().y(), object->pos().z());
                glVertex3f(subobject->pos().x(), subobject->pos().y(), subobject->pos().z());
            }
        }
    }
    glEnd();

    mFpsCounter++;
}

void GlWidget::updateInfoLabel() {
    mInfoLabel->setText(QString("FPS:  %1\n"
                                "TIME: %2").arg(mFps).arg(mWorld->time()/1000., 0, 'f', 1));
}

void GlWidget::onWorldRecalced() {
    update();
}

void GlWidget::onCustomContextMenuRequested(const QPoint &pos) {
    QMenu menu;
    QAction *action = menu.addAction("add object");
    QPoint worldPos = (pos - QPoint(width()/2, height()/2)) / mScale;
    connect(action, &QAction::triggered, this, [=]() -> void { mWorld->createObject(2000000000000000, worldPos.x(), worldPos.y(), 0, 0, 0, 0); });
    menu.exec(mapToGlobal(pos));
}
