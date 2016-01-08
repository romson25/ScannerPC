#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QPoint>
#include <QtMath>
#include <QDir>

#include "Eye/eye.h"
#include "rendermode.h"
#include "TcpServer/photogram.h"
#include "DataProcessor/messagetype.h"
#include "SetPointProcessing.h"
#include "PoissonSurfaceReconstructrion.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

namespace Ui {
class Scene;
}

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = 0);
    ~Scene();

signals:
    void message(MessageType, QString, QString = "Scene");  //--raportuj problemy i zdarzenia
    void removeOldestPhotogram();   //--usuwa stare skany które pożerają średnio 1% RAM na skan
    void modelPathChanged(QString); //--może to zmienić na getModelPath

public slots:
    void scanningStarted();     //--zmień renderMode na Skeleton
    void scanningFinished();    //--zmień renderMode na Model i wyrenderuj zrekonstruowany model
    void nextScan           (Photogram &);  //--kolejny skan
    void renderModelFromFile(QString path); //--gdy otwieram model z pliku

protected:
    void initializeGL()        Q_DECL_OVERRIDE;
    void paintGL()             Q_DECL_OVERRIDE;
    void resizeGL(int, int)    Q_DECL_OVERRIDE;

    void mouseMoveEvent (QMouseEvent *event)    Q_DECL_OVERRIDE;
    void wheelEvent     (QWheelEvent *event)    Q_DECL_OVERRIDE;
    void keyPressEvent  (QKeyEvent   *event)    Q_DECL_OVERRIDE;

private:
    Ui::Scene *ui;

    QOpenGLBuffer *axisVBO      {new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)};
    QOpenGLBuffer *modelVBO     {new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)};
    QOpenGLBuffer *modelIBO     {new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer)};
    QOpenGLBuffer *skeletonVBO  {new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)};

    QOpenGLVertexArrayObject *axisVAO       {new QOpenGLVertexArrayObject};
    QOpenGLVertexArrayObject *modelVAO      {new QOpenGLVertexArrayObject};
    QOpenGLVertexArrayObject *skeletonVAO   {new QOpenGLVertexArrayObject};

    QOpenGLShaderProgram *axisShader        {new QOpenGLShaderProgram};
    QOpenGLShaderProgram *modelShader       {new QOpenGLShaderProgram};
    QOpenGLShaderProgram *skeletonShader    {new QOpenGLShaderProgram};

    QVector<QVector3D>      axisVertices    {};
    QVector<QVector3D>      modelVertices   {};
    QVector<unsigned int>   modelIndices    {};
    std::vector<QVector3D>  skeletonVertices{};

    QMatrix4x4 modelMatrix      {};
    QMatrix4x4 projectionMatrix {};

    QMatrix4x4 modelViewMatrix  {};
    QMatrix4x4 mvpMatrix        {};
    QMatrix3x3 normalMatrix     {};

    const QString shadersPath   {QDir::currentPath()+"/../../shaders"};

    RenderMode renderMode       {RenderMode::axisAndSkeleton};

    Eye eye;

    void initializeAxisShader();
    void initializeAxis();
    void paintAxis();

    void initializeModelShader();
    void initializeModel();
    void paintModel();

    void initializeSkeletonShader();
    void initializeSkeleton();
    void paintSkeleton();

    void loadModelFromFile      (QString path);
    void loadVerticesAndNormals (aiMesh* mesh);
    void loadIndices            (aiMesh* mesh);

    void clearPreviousModel();
    void clearPreviousSkeleton();

    void    writeSkeleton       (QString path);
    QString reconstructSurface  (QString path);
};

#endif // SCENE_H
