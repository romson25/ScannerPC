#ifndef PHOTOGRAMMETRYRECONSTRUCTOR_H
#define PHOTOGRAMMETRYRECONSTRUCTOR_H

#include <QFile>
#include <QDebug>
#include <QObject>

//#include "photogramRecon.h"
#include "reconstructorbase.h"

using namespace std;

class PhotogrammetryReconstructor : public ReconstructorBase
{
    Q_OBJECT
public:
    explicit PhotogrammetryReconstructor();

public slots:
    void addImage   (const QImage &);
    void reconstructFromImages(QString pathToSetOfImages);
};

#endif // PHOTOGRAMMETRYRECONSTRUCTOR_H
