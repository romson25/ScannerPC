#include "laserreconstructor.h"

LaserReconstructor::LaserReconstructor()
{}

void LaserReconstructor::addImage   (const QImage& image)
{
    cv::Mat tmp(image.height(),image.width(), CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
    cv::cvtColor(tmp, cvImage, cv::COLOR_BGR2GRAY);

    estimate();
    emit skeletonChanged(cloud);
    static int i {0};
    qDebug()<<++i<<" LaserReconstructor.currentCloudSize: "<<cloud.length();
}
void LaserReconstructor::estimate   ()
{
    int imageWidth  = cvImage.cols;
    int imageHeight = cvImage.rows;
    int xMin { imageWidth/2     };
    int xMax { imageWidth-200   };
    int yMin { 200              };
    int yMax { imageHeight/2    };

    for(int y = yMin; y < yMax; y++)
    {
        int positionBrightestPixel = -1;
        int valueBrightestPixel {};

        for(int x = xMin; x < xMax; x++)
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
    float e = (float)(x - 1280)/256;
    float f = (float)(960 - y)/2560;

    QLineF gLine(0, 0, 35, 20);
    QLineF nLine(0, e, 35, 0 );
    QPointF xPos{};                 //--punkt przecięcia
    nLine.intersect(gLine, &xPos);  //--nadanie mu wartości
    qDebug()<<"xPos: "<<xPos<<"\te: "<<e<<"\tx: "<<x<<"\ty: "<<y;

    QLineF n(35, 0, xPos.x(), xPos.y());    //--linia od kamery do punktu przecięcia (do linii)
    float r   = 2.0 * xPos.y();             //--na podstawie trójkąta 30, 60, 90
    float yCm = f * (n.length());           //--

    float xp = r * cosAngle;
    float yp = r * sinAngle;
    float zp = yCm;

    return cloud.push_back(QVector3D(xp, zp, yp));
}
