#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QScopedPointer>
#include <QMatrix4x4>
#include "world.h"

class QLabel;
class QDial;

class GlWidget : public QGLWidget {
    Q_OBJECT
    QScopedPointer<World> mWorld;
    double mScale = 1.;
    QMatrix4x4 mMatrix;
    QQuaternion mQuanternion;
    bool mClearBuffer = true;

    QLabel *mInfoLabel;
    int mFpsCounter = 0;
    int mFps = 0;

public:
    GlWidget(QWidget *parent = nullptr);

    World *world() { return mWorld.data(); }
    double & scale() { return mScale; }
    QMatrix4x4 & matrix() { return mMatrix; }
    QQuaternion & quanternion() { return mQuanternion; }
    bool isClearBufferEnabled() { return mClearBuffer; }
    void setClearBuffer(bool clear) { mClearBuffer = clear; }

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private slots:
    void updateInfoLabel();
    void onWorldRecalced();
    void onCustomContextMenuRequested(const QPoint &pos);
};

#endif // GLWIDGET_H
