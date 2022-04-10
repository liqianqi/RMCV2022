#ifndef ARMORDECTOR_H
#define ARMORDECTOR_H
#include <opencv2/opencv.hpp>
#include<iostream>
<<<<<<< HEAD
#include "common.h"
#include "constants.h"
=======
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking.hpp>
#include <opencv2/tracking/tracker.hpp>
#include "common.h"
#include "constants.h"
#include "classifier.h"
>>>>>>> 这一版增加了数字识别，但效果不太理想
using namespace Horizon;
using namespace cv;
using namespace std;

class Led{

public:
    Led()
       :
       slope_(0), length_(0), width_(0),ratio_(0),   //初始化斜度，长度，宽度，比率，上顶点，下顶点，中心点
       top_point_(), bottom_point_(), center_()
    {};

    ~Led(){};

public:
    float slope_;
    float length_;
    float width_;
    float ratio_;
    Point2f center_;
    Point2f top_point_;
    Point2f bottom_point_;

<<<<<<< HEAD
};


class ArmorDector
{
public:
    ArmorDector(){}
=======
    float angle_;

};

const string FILENAME =  "../tools/para/";
class ArmorDector
{
public:
    ArmorDector():
        classifier(FILENAME)
    {
            
    }
>>>>>>> 这一版增加了数字识别，但效果不太理想

    unsigned short StorePicture(Mat &img);//传图片1111111111111
    void SeparateColor();
    unsigned short GetAllLed();
    unsigned short GetAllArmor(int led_num);
    Armor Select_Best(int armor_num);
    void Continuous_Show(Armor best);

    void ObtainAllArmor(vector<Armor> All_Armor);

    void AngleResolving(const unsigned short& armor_num);
    void GetImagePointData(const Armor& allArmor, std::vector<cv::Point2f>& point2D);
    void GetObjectPointData(const Armor& armor, std::vector<cv::Point3f>& point3D);
    void SolveAngle(Armor& armor, const std::vector<cv::Point2f>& point2D, const std::vector<cv::Point3f>& point3D);
    
<<<<<<< HEAD
    Point3f Getbest(){//获得装甲板1111111111111111111 
=======
    /**
     * @brief 获得的装甲板传给预测器中
     * 
     * @return 把三维坐标传出去
     */

    Point3f Getbest(){
>>>>>>> 这一版增加了数字识别，但效果不太理想
        Point3f best_point;
        best_point.x = best_armor.center3d_.x;
        best_point.y = best_armor.center3d_.y;
        best_point.z = best_armor.center3d_.z;
        return best_point;

    }; 

    enum ArraySize{
        LED_SIZE = 20,
        ARMOR_SIZE = 10
<<<<<<< HEAD
        };
=======
    };
>>>>>>> 这一版增加了数字识别，但效果不太理想


private:
    Mat src_image_;
    Mat binary_image_;
    Mat channel_image_;
    Mat aultimate_image_;
    Mat final_image_;
    Mat show_image_;

    enum Color{Blue = 1,Red = 0};

    unsigned short led_num;
<<<<<<< HEAD
    Point2f pigment_poi[9];              //储存对应坐标
    Led led_array_[LED_SIZE];            //存储led
    Armor armor_array_[ARMOR_SIZE];      //存储装甲板
    Armor select_best( int armor_num);

public:
   Armor best_armor;  //存储最优装甲板
   unsigned short armor_num_;

=======
    Point2f pigment_poi[9];             //储存对应坐标
    Led led_array_[LED_SIZE];           //存储led
    Armor armor_array_[ARMOR_SIZE];     //存储装甲板
    Armor select_best( int armor_num);

    bool last_is_have_target_ = false;
public:
   Armor best_armor;                    // 存储最优装甲板
   Armor last_armor_;                   // 上一帧的装甲板
   unsigned short armor_num_;

public:
    Classifier classifier;              // CNN分类器，用于数字识别

public:
    Ptr<TrackerKCF> tracker = TrackerKCF::create();        //跟踪器
    typedef cv::TrackerKCF TrackerToUse;

>>>>>>> 这一版增加了数字识别，但效果不太理想
public://排列组合函数区

    static bool ledCmpLength(const Led a, const Led b)     //长的优先
    {
        return a.length_ > b.length_;
    }

    static bool ledCmpSlope(const Led a, const Led b)      //竖直优先
    {
        return a.length_ < b.length_;
    }

    static bool ledCmpY(const Point2f a, const Point2f b)  //y优先
    {
        return a.y < b.y;
    }

    static bool ledCmpX(const Point2f a, const Point2f b)  //x优先
    {
        return a.x < b.x;
    }

};


#endif // ARMORDECTOR_H
