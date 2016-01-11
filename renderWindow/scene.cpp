#include "scene.h"
#include "ui_scene.h"

Scene::Scene (QWidget *parent) : QOpenGLWidget(parent), ui(new Ui::Scene)
{
    ui->setupUi(this);
}
Scene::~Scene()
{
    delete ui;
}

void Scene::updateModel     (QVector<QVector3D> &nextData,
                             QVector<unsigned int> &nextIndices)
{
    renderMode = RenderMode::axisAndModel;
    model.clear();
    model.addData(nextData, nextIndices);
    update();
}
void Scene::updateSkeleton  (QVector<QVector3D> &cloud)
{
    renderMode = RenderMode::axisAndModel;
    //wyczyść skeleton
    skeleton.addVertices(cloud);
    update();
}

void Scene::initializeGL    ()
{
    initializeOpenGLFunctions();

    glClearColor(0.0, 0.73, 0.82, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    axis.init       (shadersPath+"/axis/vertexaxis.vert",           shadersPath+"/axis/fragmentaxis.frag"           );
    model.init      (shadersPath+"/model/vertexmodel.vert",         shadersPath+"/model/fragmentmodel.frag"         );
    skeleton.init   (shadersPath+"/skeleton/vertexskeleton.vert",   shadersPath+"/skeleton/fragmentskeleton.frag"   );

    update();
}
void Scene::paintGL         ()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatrix  = eye.viewMatrix() * modelMatrix;
    mvpMatrix        = projectionMatrix * modelViewMatrix;
    normalMatrix     = modelViewMatrix.normalMatrix();

    switch (renderMode)
    {
    case RenderMode::axis:
        axis.paint(mvpMatrix);
        break;
    case RenderMode::axisAndModel:
        axis.paint(mvpMatrix);
        model.paint(mvpMatrix, modelViewMatrix, normalMatrix);
        break;
    case RenderMode::axisAndSkeleton:
        axis.paint(mvpMatrix);
        skeleton.paint(mvpMatrix);
        break;
    default:
        break;
    }
}
void Scene::resizeGL        (int width, int height)
{
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60, (float)width/height, 0.01f, 100.0f);

    update();
}

void Scene::mouseMoveEvent  (QMouseEvent *event)
{
    static QPoint pos{event->pos()};

    if(event->buttons() == Qt::LeftButton)
        eye.rotate(pos, event->pos());

    pos = event->pos();

    update();
}
void Scene::wheelEvent      (QWheelEvent *event)
{
    int e = event->angleDelta().y(); //-- przyjmuje 120 albo -120
    eye.changeRadius( (e > 0) ? Distance::closer : Distance::farther);

    update();
}
void Scene::keyPressEvent   (QKeyEvent   *event)
{
    switch (event->key())
    {
        case Qt::Key_Equal:  //--tak są u mnie klawisze + i - przy backspacie
            eye.changeRadius(Distance::farther);
            break;
        case Qt::Key_Minus:
            eye.changeRadius(Distance::closer);
            break;
        case Qt::Key_Left:
            eye.rotate(Direction::left);
            break;
        case Qt::Key_Right:
            eye.rotate(Direction::right);
            break;
        case Qt::Key_Escape:
            qApp->exit(0);
            break;
        default:
            break;
    }

    update();
}
