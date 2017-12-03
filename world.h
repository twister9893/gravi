#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include <QVector>

class Object;

class World : public QObject {
    Q_OBJECT

    const int RECALC_INTERVAL_MSEC = 10;
    double mTime = 0.;
    int mTimeFactor = 0;
    int mObjectCount = 0;
    QVector<Object*> mObjects;

public:
    static const double GRAVI_CONSTANT;

public:
    World();
    double time() const { return mTime; }
    void createObject(double mass, double x, double y, double z, double vx, double vy, double vz);
    QVector<Object*> & objects() { return mObjects; }

public slots:
    void setTimeFactor(int timeFactor) { mTimeFactor = timeFactor; }

private:
    void recalcObject(Object *inputObject);

private slots:
    void recalc();

signals:
    void recalced();

};

#endif // WORLD_H
