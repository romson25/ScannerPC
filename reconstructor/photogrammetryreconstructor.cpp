#include "photogrammetryreconstructor.h"

PhotogrammetryReconstructor::PhotogrammetryReconstructor()
{
    initFirstCamMatrix();
}

void PhotogrammetryReconstructor::addImage          (const QImage &image)
{
    static int addedPhotos {0};
    addedPhotos++;

    Mat tmp(image.height(),image.width(), CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
    Mat cvImage;
    cvtColor(tmp, cvImage, COLOR_BGR2GRAY);
//--zapisz zdjęcie na dysku
    imwrite(std::to_string(addedPhotos)+".jpg",cvImage);

    detectAndCompute(cvImage);
    if(addedPhotos > 1)
    {
        match();
        computeCamMatrix();
        triangulate();
    }
    prepareForNextImg();
    skeletonChanged(cloud);
    qDebug()<<"*********** Cloud:\n"<<cloud;
}
void PhotogrammetryReconstructor::detectAndCompute  (const Mat &image)
{
    detector->detectAndCompute( image, noArray(), keypoints.first(), descriptors.first() );
}
void PhotogrammetryReconstructor::match             ()
{
    matcher->match(descriptors.first(), descriptors.last(), matches);
    testMatches();
}
void PhotogrammetryReconstructor::computeCamMatrix  ()
{
    camMatrices.first() = Matx34d(1, 0,         0,          0,
                                  0, cosAngle, -sinAngle,   35 * cosAngle,
                                  0, sinAngle,  cosAngle,   35 * sinAngle);
}
void PhotogrammetryReconstructor::triangulate       ()
{
    QVector<Point3d> homoPoints1;
    QVector<Point3d> homoPoints2;

    const Matx33d k (1.2571849082973597e+03, 0., 6.2810023880994879e+02, 0.,
                     1.2561398522938134e+03, 4.9674395571560569e+02, 0., 0., 1. );
    const Matx33d invK = k.inv();

    foreach (Point2f p, points.first())
    {
        Point3d u(p.x, p.y, 1.0);
        Mat_<double> um = Mat_<double>(invK) * Mat_<double>(u);
        u.x = um(0);
        u.y = um(1);
        u.z = um(2);
        homoPoints1.push_back(u);
    }

    foreach (Point2f p, points.last())
    {
        Point3d u(p.x, p.y, 1.0);
        Mat_<double> um = Mat_<double>(invK) * Mat_<double>(u);
        u.x = um(0);
        u.y = um(1);
        u.z = um(2);
        homoPoints2.push_back(u);
    }

    int iMax = points.first().length();
    for(int i = 0; i < iMax; i++)
    {
        Matx34d p1 = camMatrices.first();
        Matx34d p2 = camMatrices.last();
        Point3d u1 = homoPoints1.at(i);
        Point3d u2 = homoPoints2.at(i);

        Matx43d A(u1.x*p1(2,0)-p1(0,0),	u1.x*p1(2,1)-p1(0,1),   u1.x*p1(2,2)-p1(0,2),
                  u1.y*p1(2,0)-p1(1,0),	u1.y*p1(2,1)-p1(1,1),	u1.y*p1(2,2)-p1(1,2),
                  u2.x*p2(2,0)-p2(0,0), u2.x*p2(2,1)-p2(0,1),	u2.x*p2(2,2)-p2(0,2),
                  u2.y*p2(2,0)-p2(1,0), u2.y*p2(2,1)-p2(1,1),	u2.y*p2(2,2)-p2(1,2));

        Matx41d B(-(u1.x*p1(2,3)    -p1(0,3)),
                  -(u1.y*p1(2,3)    -p1(1,3)),
                  -(u2.x*p2(2,3)	-p2(0,3)),
                  -(u2.y*p2(2,3)	-p2(1,3)));

        Mat_<double> X;
        solve(A,B,X,DECOMP_SVD);

        cloud.push_back( QVector3D(X(0), X(1), X(2)) );
    }
}

void PhotogrammetryReconstructor::testMatches       ()
{
    int iMax { (int)matches.size() };
    for(int i {0}; i < iMax; i++)
    {
        DMatch match { matches.at(i) };

        if(match.distance < 50)
        {
            Point2d point1 { keypoints.first().at(match.queryIdx).pt };
            Point2d point2 { keypoints.last().at(match.trainIdx).pt  };

            double horizontalDifference  { qAbs(point1.x - point2.x) };
            double verticalDifference    { qAbs(point1.y - point2.y) };

            if( horizontalDifference < 100  )
                if( verticalDifference < 10 )
                {
                    points.first().push_back( point1 );
                    points.last().push_back( point2  );
                }
        }
    }
}
void PhotogrammetryReconstructor::prepareForNextImg ()
{
    matches.clear();
    descriptors.pop_back();
    descriptors.push_front( Mat() );
    keypoints.pop_back();
    keypoints.push_front( vector<KeyPoint> () );
    points.pop_back();
    points.push_front( QVector<Point2d> () );
    camMatrices.pop_back();
    camMatrices.push_front( Matx34d() );
}
void PhotogrammetryReconstructor::initFirstCamMatrix()
{
    camMatrices.first() =  Matx34d{ 1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0 };
}
