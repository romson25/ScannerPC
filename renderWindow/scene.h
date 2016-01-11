#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLWidget>

#include <QVector>
#include <QVector3D>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QPoint>
#include <QtMath>
#include <QDir>

#include "renderObject/axis/axis.h"
#include "renderObject/model/model.h"
#include "renderObject/skeleton/skeleton.h"

#include "eye/eye.h"
#include "rendermode.h"

#include "DataProcessor/messagetype.h"

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
    void message            (MessageType, QString, QString = "Scene");  //--raportuj problemy i zdarzenia
    void modelPathChanged   (QString);  //--może to zmienić na getModelPath

public slots:
    void updateModel        (QVector<QVector3D> &,
                             QVector<unsigned int> &);
    void updateSkeleton     (QVector<QVector3D> &   );

protected:
    void initializeGL   ()          Q_DECL_OVERRIDE;
    void paintGL        ()          Q_DECL_OVERRIDE;
    void resizeGL       (int, int)  Q_DECL_OVERRIDE;

    void mouseMoveEvent (QMouseEvent *event)    Q_DECL_OVERRIDE;
    void wheelEvent     (QWheelEvent *event)    Q_DECL_OVERRIDE;
    void keyPressEvent  (QKeyEvent   *event)    Q_DECL_OVERRIDE;

private:
    Ui::Scene *ui;

    Eye eye;
    Axis        axis    {this};
    Model       model   {this};
    Skeleton    skeleton{this};

    QMatrix4x4 modelMatrix      {};
    QMatrix4x4 projectionMatrix {};

    QMatrix4x4 modelViewMatrix  {};
    QMatrix4x4 mvpMatrix        {};
    QMatrix3x3 normalMatrix     {};

    const QString shadersPath   {QDir::currentPath()+"/../../renderWindow/renderObject"};

    RenderMode renderMode       {RenderMode::axisAndSkeleton};
};

#endif // SCENE_H
