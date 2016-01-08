#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "../irenderobject.h"

class Model : public QObject, public IRenderObject
{
    Q_OBJECT
    Q_INTERFACES(IRenderObject)

public:
    explicit Model(QOpenGLFunctions* f);
    ~Model(){}

    void init   (QString vertexShaderPath, QString fragemntShaderPath);
    void paint  (const QMatrix4x4& mvpMatrix,
                 const QMatrix4x4& modelViewMatrix  = QMatrix4x4(),
                 const QMatrix3x3& normalMatrix     = QMatrix3x3() );

    void addVertices    (QVector3D vertex, QVector3D normal);
    void addIndex       (unsigned int index);
    void refreshVertices();
    void refreshIndices ();
    void clear          ();

private:
    void initObject ();
    void initShader (QString vertexShaderPath, QString fragemntShaderPath);

    QOpenGLBuffer *ibo {new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer)};

    QVector<QVector3D> vertices;
    QVector<unsigned int> indices;
    QOpenGLFunctions* f;
};

#endif // MODEL_H
