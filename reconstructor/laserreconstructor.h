#ifndef LASERRECONSTRUCTOR_H
#define LASERRECONSTRUCTOR_H

#include "reconstructorbase.h"

class LaserReconstructor : public ReconstructorBase
{
    Q_OBJECT
public:
    explicit LaserReconstructor();

public slots:
    void addImage(const QImage &);

private:
        void estimate ();
        void mathCalc (int x, int y);

        const int threshold {70};
};

#endif // LASERRECONSTRUCTOR_H
