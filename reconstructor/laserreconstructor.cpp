#include "laserreconstructor.h"

LaserReconstructor::LaserReconstructor()
{}

void LaserReconstructor::addImage   (const QImage& image)
{
    cv::Mat tmp(image.height(),image.width(), CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
    cv::cvtColor(tmp, cvImage, cv::COLOR_BGR2GRAY);
    estimate();
    emit skeletonChanged(cloud);
}
void LaserReconstructor::setAngle   (float a)
{
    sinAngle = qSin( qDegreesToRadians(a) );
    cosAngle = qCos( qDegreesToRadians(a) );
}
void LaserReconstructor::estimate   ()
{
    int imageWidth      = cvImage.cols;
    int imageHalfWidth  = cvImage.cols / 2;
    int imageHalfHeight = cvImage.rows / 2;
    const int threshold {100};

    for(int y = 0; y < imageHalfHeight; y++)
    {
        int positionBrightestPixel = -1;
        int valueBrightestPixel {};

        for(int x = imageHalfWidth; x < imageWidth; x++)
        {
            uchar brightness{cvImage.at<uchar>(y,x)};

            if(brightness > valueBrightestPixel)
            {
                valueBrightestPixel = brightness;
                positionBrightestPixel = x;
            }
        }

        if(valueBrightestPixel > threshold)
            mathCalc(positionBrightestPixel,y);
    }
}
void LaserReconstructor::mathCalc   (int x, int y)
{
    float e = (float)(x - 1280)/128;
    float f = (float)(960 - y)/2560;

    QLineF gLine(0, 0, 20, 11.5);
    QLineF nLine(20, 0, 0, e);
    QPointF xPos{};                 //--punkt przecięcia
    nLine.intersect(gLine, &xPos);  //--nadanie mu wartości

    QLineF n(20, 0, xPos.x(), xPos.y());    //--linia od kamery do punktu przecięcia (do linii)
    float r   = 2.0 * xPos.y();             //--na podstawie trójkąta 30, 60, 90
    float yCm = f * (n.length());           //--

    float xp = r * cosAngle;
    float yp = r * sinAngle;
    float zp = yCm;

    return cloud.push_back(QVector3D(xp, zp, yp));
}
