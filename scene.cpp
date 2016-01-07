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
    std::move(p.getVertices().begin(), p.getVertices().end(), std::back_inserter(skeletonVertices));
    skeletonVBO->bind();
    glBufferData(GL_ARRAY_BUFFER, skeletonVertices.size()*sizeof(QVector3D), skeletonVertices.data(), GL_STATIC_DRAW);
    skeletonVBO->release();

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

    initializeAxis();
    initializeAxisShader();

    initializeModel();
    initializeModelShader();

    initializeSkeleton();
    initializeSkeletonShader();

    update();
}
void Scene::paintGL         ()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatrix  = eye.viewMatrix() * modelMatrix;
    mvpMatrix        = projectionMatrix * modelViewMatrix;
    normalMatrix     = modelViewMatrix.normalMatrix();

    switch (renderMode) {
    case RenderMode::axis:
        paintAxis();
        break;
    case RenderMode::axisAndModel:
        paintAxis();
        paintModel();
        break;
    case RenderMode::axisAndSkeleton:
        paintAxis();
        paintSkeleton();
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

void Scene::initializeAxisShader    ()
{
    axisShader->create();
    axisShader->addShaderFromSourceFile(QOpenGLShader::Vertex,      shadersPath + "/vertexaxis.vert");
    axisShader->addShaderFromSourceFile(QOpenGLShader::Fragment,    shadersPath + "/fragmentaxis.frag");
    axisShader->bindAttributeLocation("position",   0);
    axisShader->bindAttributeLocation("color",      1);
    axisShader->link();
    axisShader->bind();
}
void Scene::initializeAxis          ()
{
    axisVertices.push_back(QVector3D{ 0, 0, 0});
    axisVertices.push_back(QVector3D{ 1, 0, 0});
    axisVertices.push_back(QVector3D{10, 0, 0});
    axisVertices.push_back(QVector3D{ 1, 0, 0});
    axisVertices.push_back(QVector3D{ 0, 0, 0});
    axisVertices.push_back(QVector3D{ 0, 1, 0});
    axisVertices.push_back(QVector3D{ 0,10, 0});
    axisVertices.push_back(QVector3D{ 0, 1, 0});
    axisVertices.push_back(QVector3D{ 0, 0, 0});
    axisVertices.push_back(QVector3D{ 0, 0, 1});
    axisVertices.push_back(QVector3D{ 0, 0,10});
    axisVertices.push_back(QVector3D{ 0, 0, 1});

    axisVAO->create();
    axisVAO->bind();

    axisVBO->create();
    axisVBO->bind();
    axisVBO->allocate(axisVertices.data(), axisVertices.length()*sizeof(QVector3D));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    axisVBO->release();
    axisVAO->release();
}
void Scene::paintAxis               ()
{
    axisVAO->bind();
    axisShader->bind();

    axisShader->setUniformValue("mvp", mvpMatrix);

    glDrawArrays(GL_LINES, 0, axisVertices.length());

    axisShader->release();
    axisVAO->release();
}

void Scene::initializeModelShader   ()
{
     modelShader->create();
     modelShader->addShaderFromSourceFile(QOpenGLShader::Vertex,    shadersPath + "/vertexmodel.vert");
     modelShader->addShaderFromSourceFile(QOpenGLShader::Fragment,  shadersPath + "/fragmentmodel.frag");
     modelShader->bindAttributeLocation("position", 0);
     modelShader->bindAttributeLocation("normal",   1);
     modelShader->link();
     modelShader->bind();
}
void Scene::initializeModel         ()
{
    modelVAO->create();
    modelVAO->bind();

    modelVBO->create();
    modelIBO->create();

    modelVAO->release();
}
void Scene::paintModel              ()
{
    modelVAO->bind();
    modelVBO->bind();
    modelShader->bind();

    modelShader->setUniformValue("modelViewMatrix", modelViewMatrix);
    modelShader->setUniformValue("mvp",             mvpMatrix);
    modelShader->setUniformValue("normalMatrix",    normalMatrix);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<void *>(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    modelIBO->bind();
    glDrawElements(GL_TRIANGLES, modelIndices.length(), GL_UNSIGNED_INT, 0);

    modelIBO->release();
    modelShader->release();
    modelVBO->release();
    modelVAO->release();
}

void Scene::initializeSkeletonShader()
{
    skeletonShader->create();
    skeletonShader->addShaderFromSourceFile(QOpenGLShader::Vertex,    shadersPath + "/vertexskeleton.vert");
    skeletonShader->addShaderFromSourceFile(QOpenGLShader::Fragment,  shadersPath + "/fragmentskeleton.frag");
    skeletonShader->bindAttributeLocation("position", 0);
    skeletonShader->link();
    skeletonShader->bind();
}
void Scene::initializeSkeleton      ()
{
    skeletonVAO->create();
    skeletonVAO->bind();

    skeletonVBO->create();
    skeletonVBO->bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);

    skeletonVBO->release();
    skeletonVAO->release();
}
void Scene::paintSkeleton           ()
{
    skeletonVAO->bind();
    skeletonShader->bind();

    skeletonShader->setUniformValue("mvp", mvpMatrix);

    glDrawArrays(GL_POINTS, 0, skeletonVertices.size());

    skeletonShader->release();
    skeletonVAO->release();
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

        modelVertices.push_back(QVector3D(xV, yV, zV));
        modelVertices.push_back(QVector3D(xN, yN, zN));
    }

    modelVBO->bind();
    glBufferData(GL_ARRAY_BUFFER, modelVertices.size()*sizeof(QVector3D), modelVertices.data(), GL_STATIC_DRAW);
    modelVBO->release();
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

        modelIndices.push_back(index1);
        modelIndices.push_back(index2);
        modelIndices.push_back(index3);
    }

    modelIBO->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelIndices.length()*sizeof(unsigned int), modelIndices.data(), GL_STATIC_DRAW);
    modelIBO->release();
}

void Scene::clearPreviousModel      ()
{
    modelIndices.clear();
    modelVertices.clear();
}
void Scene::clearPreviousSkeleton   ()
{
    skeletonVertices.clear();
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

    for(unsigned int i = 0; i<skeletonVertices.size(); i++)
    {
        QVector3D v = skeletonVertices.at(i);

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
