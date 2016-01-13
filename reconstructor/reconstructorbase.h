#ifndef RECONSTRUCTORBASE_H
#define RECONSTRUCTORBASE_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QtMath>
#include <QTextStream>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

//#include "SetPointProcessing.h"
//#include "PoissonSurfaceReconstructrion.h"

class ReconstructorBase : public QObject
{
    Q_OBJECT
public:
    explicit ReconstructorBase(QObject *parent = 0);
    virtual ~ReconstructorBase();

signals:
    void modelChanged   (QVector<QVector3D> &,
                         QVector<unsigned int> &);
    void skeletonChanged(QVector<QVector3D> &);

public slots:
    virtual void addImage   (const QImage &) =0;

    void scanningStarted    ();
    void scanningFinished   ();
    void loadModel  (QString path);
    void saveCloud  (QString path);
    void setAngle   (float);

protected:
    void setData    (aiMesh *mesh);     //--ustawia w Data wierzchołki i normalne
    void setIndices (aiMesh *mesh);     //--ustawia w Indices indeksy wierzchołków
    void reconstruct();                 //--rekonstrukcja modelu z cloud
    void clear      ();                 //--czyści zawartość cloud/data/indices

    QVector<QVector3D>      cloud   {}; //--zawiera czyste wiechołki (szkielet/szkic)
    QVector<QVector3D>      data    {}; //--zawiera naprzemiennie wierzchołki/normalne
    QVector<unsigned int>   indices {}; //--indeksy
    cv::Mat                 cvImage {};

    float sinAngle {};
    float cosAngle {};
};

#endif // RECONSTRUCTORBASE_H
