#ifndef SKELETON_H
#define SKELETON_H

#include <QObject>
#include "../irenderobject.h"

class Skeleton : public QObject, public IRenderObject
{
    Q_OBJECT
    Q_INTERFACES(IRenderObject)
public:
    explicit Skeleton(QOpenGLFunctions* f);
    ~Skeleton(){}

    void init   (QString vertexShaderPath, QString fragemntShaderPath);
    void paint  (const QMatrix4x4& mvpMatrix,
                 const QMatrix4x4& modelViewMatrix  = QMatrix4x4(),
                 const QMatrix3x3& normalMatrix     = QMatrix3x3() );

        void addVertices(std::vector<QVector3D>& vertices);
        void clear      ();
        const QVector<QVector3D>&  getVertices();

private:
    void initObject ();
    void initShader (QString vertexShaderPath, QString fragemntShaderPath);

    QVector<QVector3D> vertices;
    QOpenGLFunctions* f;
};

#endif // SKELETON_H
