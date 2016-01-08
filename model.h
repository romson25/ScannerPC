#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "renderobject.h"

class Model : public QObject, public IRenderObject
{
    Q_OBJECT
    Q_INTERFACES(IRenderObject)

public:
    explicit Model(QOpenGLFunctions* f);
    ~Model(){}

    void init   (QString vertexShaderPath, QString fragemntShaderPath);
    void paint  (const QMatrix4x4& mvpMatrix,
                 const QMatrix4x4& modelViewMatrix = QMatrix4x4(),
                 const QMatrix3x3& normalMatrix = QMatrix3x3() );

private:
    void initObject ();
    void initShader (QString vertexShaderPath, QString fragemntShaderPath);

    QVector<QVector3D> vertices;
    QOpenGLFunctions* f;
};

#endif // MODEL_H
