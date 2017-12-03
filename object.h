#ifndef OBJECT_H
#define OBJECT_H

#include <QtMath>
#include <QLineF>
#include <QVector3D>
#include "world.h"

class Object {
    friend class World;

    const int ID;
    const double MASS;

    QVector3D mPos;
    QVector3D mSpeed;
    QVector3D mWorldAccel;

public:
    int id() const { return ID; }
    double mass() const { return MASS; }

    QVector3D & pos() { return mPos; }
    QVector3D & speed() { return mSpeed; }
    QVector3D & worldAccel() { return mWorldAccel; }

    QVector3D vectorTo(Object *object) {
        double acceleration = World::GRAVI_CONSTANT * object->mass() /
                              qPow(mPos.distanceToPoint(object->pos()), 2);
        return QVector3D(object->pos() - mPos).normalized() * acceleration;
    }


private:
    Object(int id, double mass, double x, double y, double z, double vx, double vy, double vz)
        : ID(id),
          MASS(mass),
          mPos(QVector3D(x,y,z)),
          mSpeed(vx,vy,vz) { }
};

#endif // OBJECT_H
