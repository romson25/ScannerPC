#include "scene.h"
#include "ui_scene.h"

Scene::Scene(QWidget *parent) : QOpenGLWidget(parent), ui(new Ui::Scene)
{
    ui->setupUi(this);
}

Scene::~Scene()
{
    delete ui;
}

void Scene::scanningStarted     ()
{
    renderMode = RenderMode::axisAndSkeleton;
    clearPreviousSkeleton();
    emit message(MessageType::log, "Scena przygotowana do skanowania");
}
void Scene::scanningFinished    ()
{
    renderMode = RenderMode::axisAndModel;

    QString path = "model.xyz";
    writeSkeleton(path);

    QString pathToModel = reconstructSurface(path);
    modelPathChanged(pathToModel);
    renderModelFromFile(pathToModel);
//    renderModelFromFile("/home/adam/Desktop/ScannerPC/build/model_model.off");

    emit message(MessageType::log, "Koniec skanowania. Rozpoczęto procedurę rekonstrucji obiektu");
}
void Scene::nextScan            (Photogram &p)
{
    skeleton.addVertices( p.getVertices() );
    emit removeOldestPhotogram();
    update();
}
void Scene::renderModelFromFile (QString path)
{
    renderMode = RenderMode::axisAndModel;
    clearPreviousModel();
    loadModelFromFile(path);

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
void Scene::keyPressEvent   (QKeyEvent *event)
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

void Scene::loadModelFromFile       (QString path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toStdString(), aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

    if(!scene)
    {
        QString error = importer.GetErrorString();
        emit message(MessageType::criticalError, "Nie można otworzyć pliku z modelem: " + error);
        return;
    }

    aiMesh *mesh = scene->mMeshes[0];
    loadVerticesAndNormals(mesh);
    loadIndices(mesh);

    emit message(MessageType::log, "Załadowano model z pliku: "+path);
}
void Scene::loadVerticesAndNormals  (aiMesh *mesh)
{
    if(!mesh->HasPositions() || !mesh->HasNormals())
    {
        emit message(MessageType::criticalError, "Nie można odczytać współrzędnych wierzchołków i wektorów normalnych");
        return;
    }

    int vertexLength = mesh->mNumVertices;

    for(int i=0; i<vertexLength; i++)
    {
        aiVector3D vertex{mesh->mVertices[i]};
        aiVector3D normal{mesh->mNormals[i]};

        float xV = vertex.x;
        float yV = vertex.y;
        float zV = vertex.z;

        float xN = normal.x;
        float yN = normal.y;
        float zN = normal.z;

        model.addVertices(QVector3D(xV, yV, zV), QVector3D(xN, yN, zN));
    }

    model.refreshVertices();
}
void Scene::loadIndices             (aiMesh *mesh)
{
    if(!mesh->HasFaces())
    {
        emit message(MessageType::criticalError, "Nie można odczytać indeksów");
        return;
    }

    int indexLength = mesh->mNumFaces;

    for(int i = 0; i<indexLength; i++)
    {
        aiFace face{mesh->mFaces[i]};

        unsigned int index1 = face.mIndices[0];
        unsigned int index2 = face.mIndices[1];
        unsigned int index3 = face.mIndices[2];

        model.addIndex(index1);
        model.addIndex(index2);
        model.addIndex(index3);
    }
    model.refreshIndices();
}

void Scene::clearPreviousModel      ()
{
    model.clear();
}
void Scene::clearPreviousSkeleton   ()
{
    skeleton.clear();
}

void Scene::writeSkeleton           (QString path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"Problem z otwarciem pliku";
        return;
    }

    QTextStream stream(&file);

    foreach (QVector3D v, skeleton.getVertices())
    {
        QByteArray x = QByteArray::number(v.x());
        QByteArray y = QByteArray::number(v.y());
        QByteArray z = QByteArray::number(v.z());

        stream<<x+" "+y+" "+z+"\n";
    }

    file.flush();
    file.close();
}
QString Scene::reconstructSurface   (QString path)
{
    SetPointProcessing s(path.toStdString());
    std::string tempPath = s.compute();

    PoissonSurfaceReconstructrion p(tempPath);
    QString pathToModel = QString::fromStdString(p.reconstruct());

    return pathToModel;
}
