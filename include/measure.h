#ifndef MEASURE_H
#define MEASURE_H

#include <opencv2/opencv.hpp>
#include <vector>

//#include "common.h"

using namespace std;
using namespace cv;
class MeasureDistance{
public:
    void getPoint2D(vector<Point2f> &target);

    void getPoint3D();

    void getPoint3D1();
    Point3f pnpResolving(vector<Point2f> &target);
private:
    //单目大符相机内参
    const cv::Mat caremaMatrix = (
        cv::Mat_<float>(3, 3) <<
            1007.71031680782,    1.06891167897360,      622.027728537478,
                0,               1345.23165046327,      667.300428891571,
                0,               0,                             1);
    //畸变参数
    const cv::Mat distCoeffs = (cv::Mat_<float>(1, 5) << 0.0180891111150545,
                                                         0.0209453916251860,
                                                         0.00363657514369072,
                                                        0.00137452978133387,
                                                        -0.186986366245217);

    vector<Point2f>  point2D;
    vector<Point3f>  point3D;
};



#endif // MEASURE_H
