#ifndef PHOTOGRAMMETRYRECONSTRUCTOR_H
#define PHOTOGRAMMETRYRECONSTRUCTOR_H

#include <QObject>

class PhotogrammetryReconstructor : public QObject
{
    Q_OBJECT
public:
    explicit PhotogrammetryReconstructor(QObject *parent = 0);

signals:

public slots:
};

#endif // PHOTOGRAMMETRYRECONSTRUCTOR_H
