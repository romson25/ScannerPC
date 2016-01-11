#ifndef AXIS_H
#define AXIS_H

#include <QObject>
#include "../irenderobject.h"

class Axis : public QObject, public IRenderObject
{
    Q_OBJECT
    Q_INTERFACES(IRenderObject)
public:
    explicit Axis(QOpenGLFunctions* f);
    ~Axis(){}

    void init   (QString vertexShaderPath, QString fragemntShaderPath);
    void paint  (const QMatrix4x4& mvpMatrix,
                 const QMatrix4x4& modelViewMatrix  = QMatrix4x4(),
                 const QMatrix3x3& normalMatrix     = QMatrix3x3() );
    void clear  ();

private:
    void initObject ();
    void initShader (QString vertexShaderPath, QString fragemntShaderPath);

        QVector<QVector3D> vertices;
        QOpenGLFunctions* f;
};

#endif // AXIS_H
