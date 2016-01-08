#ifndef EYE_H
#define EYE_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>

#include "distance.h"
#include "direction.h"

class Eye : public QObject
{
    Q_OBJECT
public:
    explicit Eye(QObject *parent = 0);

    void changeRadius(Distance);
    void rotate(Direction);
    void rotate(QPoint, QPoint);

    QMatrix4x4 viewMatrix();

private:
    QVector3D position    {};
    QVector3D direction   {};
    QVector3D up          {0.0, 1.0, 0.0};

    QMatrix4x4 matrixView {};

    float r       {10.0};
    float phi     {0.5};
    float theta   {0.5};

    const QVector3D versorY {0.0, 1.0, 0.0};
    const float     rotateUnit  = qDegreesToRadians(1.0);
    const float     maxTheta    = qDegreesToRadians(89.0); //--przy 90 nic nie widać

    void refreshMatrixView ();
    QVector3D refreshPosition();
};

#endif // EYE_H
