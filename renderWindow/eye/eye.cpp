#include "eye.h"

Eye::Eye(QObject *parent) : QObject(parent)
{

}

void Eye::changeRadius(Distance distance)
{
    if(distance == Distance::closer)
        r *= 0.9;
    else
        r *= 1.1;
}
void Eye::rotate(Direction direction)
{
    direction == Direction::left ? phi -= rotateUnit : phi += rotateUnit;
}
void Eye::rotate(QPoint oldPos, QPoint newPos)
{
//------------------------------------------------Rotacja kamery    lewo - prawo
    if(newPos.x() - oldPos.x() > 0)
        phi -= rotateUnit;
    else if(newPos.x() - oldPos.x() < 0)
        phi += rotateUnit;

//------------------------------------------------Rotacja kamery     do góry - w dół
    if(newPos.y() - oldPos.y() > 0)
        theta -= rotateUnit;
    else if(newPos.y() - oldPos.y() < 0)
        theta += rotateUnit;

//------------------------------------------------Sprawdzenie poprawności kąta theta
    if(theta > maxTheta)
        theta = maxTheta;
    else if(theta < 0)
        theta = 0;
}

QMatrix4x4 Eye::viewMatrix()
{
    refreshMatrixView();
    return matrixView;
}

void Eye::refreshMatrixView()
{
    position = refreshPosition();

    matrixView.setToIdentity();
    matrixView.lookAt(position, direction, up);
}
QVector3D Eye::refreshPosition()
{
    float sinPhi   = qSin(phi);
    float cosPhi   = qCos(phi);
    float sinTheta = qSin(theta);
    float cosTheta = qCos(theta);

    float x = r * cosTheta * cosPhi;
    float y = r * cosTheta * sinPhi;
    float z = r * sinTheta;

    return QVector3D{x, z, y};
}
