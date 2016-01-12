#include "reconstructorbase.h"

ReconstructorBase::ReconstructorBase(QObject *parent) : QObject(parent)
{}
ReconstructorBase::~ReconstructorBase()
{}

void ReconstructorBase::scanningStarted ()
{
//--kiedy zaczynasz nowe skanowanie usuń stare dane
    clear();
}
void ReconstructorBase::scanningFinished()
{
//--po zakończeniu skanowania zrekonstruuj scene jeżeli chmura jest wystarczjąco duża
    if(cloud.length() > 1000)
        reconstruct();
    else
        qDebug()<<"Rekonstrukcja nie zostanie przeprowadzona, chmura jest zbyt mała";
}

void ReconstructorBase::loadModel   (QString path)
{
    clear();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toStdString(),
                                             aiProcess_GenNormals |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_Triangulate);

    if(!scene)
    {
        qDebug()<<"Nie można otworzyć pliku z modelem: ";
        return;
    }

    aiMesh *mesh = scene->mMeshes[0];
    setData(mesh);
    setIndices(mesh);

    emit modelChanged(data, indices);

    qDebug()<<"ReconstructorBase::loadModel::"
              "Załadowano model z pliku: "+path;
}
void ReconstructorBase::saveCloud   (QString path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"ReconstructorBase::saveCloud::Problem z otwarciem pliku";
        return;
    }

    QTextStream stream(&file);

    foreach (QVector3D v, cloud)
    {
        QByteArray x = QByteArray::number(v.x());
        QByteArray y = QByteArray::number(v.y());
        QByteArray z = QByteArray::number(v.z());

        stream<<x+" "+y+" "+z+"\n";
    }

    file.flush();
    file.close();
}
void ReconstructorBase::setData     (aiMesh *mesh)
{
    if(!mesh->HasPositions() || !mesh->HasNormals())
    {
        qDebug()<<"ReconstructorBase::loadModel::"
                  "Nie można odczytać wierzchołków i normalnych";
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

        data.push_back( QVector3D(xV, yV, zV) );
        data.push_back( QVector3D(xN, yN, zN) );
    }
}
void ReconstructorBase::setIndices  (aiMesh *mesh)
{
    if(!mesh->HasFaces())
    {
        qDebug()<<"ReconstructorBase::setIndices::Nie można odczytać indeksów";
        return;
    }

    int indexLength = mesh->mNumFaces;

    for(int i = 0; i<indexLength; i++)
    {
        aiFace face{mesh->mFaces[i]};

        indices.push_back( face.mIndices[0] );
        indices.push_back( face.mIndices[1] );
        indices.push_back( face.mIndices[2] );
    }
}
void ReconstructorBase::reconstruct ()
{
//--zapisz chmurę punktów (skeleton)
    const QString path {"model.xyz"};
    saveCloud(path);

//--zoptymalizuj ją i wyznacz normalne
    SetPointProcessing s(path.toStdString());
    std::string tempPath = s.compute();

//--zrekonstruuj model i zapisz do pliku
    PoissonSurfaceReconstructrion p(tempPath);
    QString pathToModel = QString::fromStdString( p.reconstruct(30.0, 2.0, 0.2) );

//--wczytaj ten model i daj znać, że możesz renderować
    loadModel(pathToModel);
    emit modelChanged(data, indices);
}
void ReconstructorBase::clear       ()
{
    cloud.clear();
    data.clear();
    indices.clear();
}
