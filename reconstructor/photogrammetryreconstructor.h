#ifndef PHOTOGRAMMETRYRECONSTRUCTOR_H
#define PHOTOGRAMMETRYRECONSTRUCTOR_H

#include <QObject>
#include <opencv2/features2d.hpp>

#include "reconstructorbase.h"

using namespace cv;
using namespace std;

class PhotogrammetryReconstructor : public ReconstructorBase
{
    Q_OBJECT
public:
    explicit PhotogrammetryReconstructor();

public slots:
    void addImage(const QImage &);

private:
    void detectAndCompute   (const Mat &);
    void match              ();
    void computeCamMatrix   ();
    void triangulate        ();

    void testMatches        ();
    void prepareForNextImg  ();
    void initFirstCamMatrix ();

    float ratio;
    Ptr<DescriptorMatcher>  matcher     { new BFMatcher(NORM_HAMMING2, true) };
    Ptr<Feature2D>          detector    { ORB::create(2000, 1.2f, 8, 50, 0, 4,
                                                     ORB::HARRIS_SCORE, 50, 50) };
    vector<DMatch>          matches     {};
    QVector<Mat>                descriptors { {}, {} };
    QVector<vector<KeyPoint>>   keypoints   { {}, {} };
    QVector<QVector<Point2d>>   points      { {}, {} };
    QVector<Matx34d>            camMatrices { {}, {} };
};

#endif // PHOTOGRAMMETRYRECONSTRUCTOR_H
