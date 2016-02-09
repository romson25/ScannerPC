#ifndef LASERRECONSTRUCTOR_H
#define LASERRECONSTRUCTOR_H

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "reconstructorbase.h"

using namespace cv;

class LaserReconstructor : public ReconstructorBase
{
    Q_OBJECT
public:
    explicit LaserReconstructor();

signals:
    void skeletonChanged(QVector<QVector3D> &);

public slots:
    void addImage(const QImage &);
    void setAngle(float);

private:
        void estimate ();
        void mathCalc (int x, int y);

        const int threshold {70};
        cv::Mat cvImage {};

        float sinAngle {};
        float cosAngle {};
};

#endif // LASERRECONSTRUCTOR_H
