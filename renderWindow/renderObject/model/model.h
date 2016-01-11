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
    void clear  ();

        void addData(const QVector<QVector3D> &nextData,
                 const QVector<unsigned int> &nextIndices);

private:
    void initObject ();
    void initShader (QString vertexShaderPath, QString fragemntShaderPath);

        QOpenGLBuffer *ibo {new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer)};
        QOpenGLFunctions* f;

        QVector<QVector3D>      data    {};
        QVector<unsigned int>   indices {};
};

#endif // MODEL_H
