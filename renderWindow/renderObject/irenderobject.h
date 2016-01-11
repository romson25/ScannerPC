#ifndef IRenderObject_H
#define IRenderObject_H

#include <QObject>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLContext>

class IRenderObject
{
public:
    virtual ~IRenderObject(){}

    virtual void init(QString vertexShaderPath,
                      QString fragmentShaderPath) =0;

    virtual void paint(const QMatrix4x4& mvpMatrix,
                       const QMatrix4x4& modelViewMatrix = QMatrix4x4(),
                       const QMatrix3x3& normalMatrix    = QMatrix3x3() )=0;
    virtual void clear()=0;

protected:
    QOpenGLBuffer               vbo     {};
    QOpenGLVertexArrayObject    vao     {};
    QOpenGLShaderProgram        shader  {};

    virtual void initObject ()=0;
    virtual void initShader (QString vertexShaderPath,
                             QString fragmentShaderPath) =0;
};

Q_DECLARE_INTERFACE(IRenderObject,"com.bruceCL.IRenderObject/1.0")

#endif // IRenderObject_H
