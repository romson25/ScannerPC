#ifndef LASERRECONSTRUCTOR_H
#define LASERRECONSTRUCTOR_H

#include <QtMath>
#include "reconstructorbase.h"

class LaserReconstructor : public ReconstructorBase
{
    Q_OBJECT
public:
    explicit LaserReconstructor();

public slots:
    void addImage(const QImage &);

        void setAngle(float);

private:
        void estimate ();
        void mathCalc (int x, int y);

        float sinAngle {};
        float cosAngle {};
        const int threshold {50};
};

#endif // LASERRECONSTRUCTOR_H
