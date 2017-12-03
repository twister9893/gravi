#include "world.h"

#include <QDebug>
#include <QTimer>
#include "object.h"

World::World() {
    QTimer *recalcTimer = new QTimer(this);
    connect(recalcTimer, &QTimer::timeout, this, &World::recalc);
    recalcTimer->start(RECALC_INTERVAL_MSEC);
}

void World::createObject(double mass, double x, double y, double z, double vx, double vy, double vz) {
    mObjects.append(new Object(++mObjectCount, mass, x, y, z, vx, vy, vz));
}

void World::recalcObject(Object *inputObject) {
    Q_ASSERT(inputObject);
    inputObject->pos() += inputObject->speed() * RECALC_INTERVAL_MSEC/1000. * mTimeFactor;

    inputObject->worldAccel() = QVector3D();
    for(Object *object : mObjects) {
        if(object->id() != inputObject->id()) {
            inputObject->worldAccel() += inputObject->vectorTo(object);
        }
    }
    inputObject->speed() += inputObject->worldAccel() * RECALC_INTERVAL_MSEC/1000. * mTimeFactor;
}

void World::recalc() {
    mTime += RECALC_INTERVAL_MSEC * mTimeFactor;
    for(Object *object : mObjects) {
        recalcObject(object);
    }
    emit recalced();
}

const double World::GRAVI_CONSTANT = 0.00000000006674;
