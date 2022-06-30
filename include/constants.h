#ifndef CONSTANTS
#define CONSTANTS
#include<opencv2/opencv.hpp>

namespace contants {



//曝光时
static const unsigned short ExposureTime = 2000;
//曝光增益
static const unsigned short ExposureGain = 300;

//二值化阈值
static const unsigned short BinaryRange = 80;
static const unsigned short Gamma = 0;

//灯条确定
//灯条最小像素值
static const unsigned int MinLedArea = 15;
// 灯条最大像素值
static const unsigned int MaxLedArea = 3e3;
//灯条斜率范围 0~x   灯条斜率^-1

static const float kLedMaxSlope = 1.0;
//长宽比范围
static const float kLedMinHeightVSWidth = 1.0;
static const float kLedMaxHeightVSWidth = 14.0;

const double kArmorTypeThreshold = 0.275;


//装甲板确定
//长度之比最大 0~x
static const float LengthRatio = 1.3;
//角度差误差范围 0~x
static const float AngleRatioDelta = 0.1;
//灯条中心Y方向距离相对灯条长度 0~x
static const float DeltaYRatio = 0.4;
//灯条中心X方向距离相对灯条长度 0~x
static const float DeltaXRatio = 3.0;

//最小长宽比
static const float ArmorMinRatio = 0.55;
//最大长宽比
static const float ArmorMaxRatio = 16.0;

//丢帧限制
static const unsigned int LostRange = 43;
//最大缓冲
static const unsigned int LostCountBuffer = 30;

//目标确定为同一个的距离最大值
static const unsigned int RangeOfCorrect = 300;
//射击角度
static const unsigned int RangeOfShoot = 50;
//----------------------------固定参数---------------------------------



static const float shootVelocityLevel_0 = 22;
static const float shootVelocityLevel_1 = 10;
static const float shootVelocityLevel_2 = 13;
static const float shootVelocityLevel_3 = 28;

//摄像头相对于枪口的距离
static const float camera_shoot_distance = 0.0f;
//枪口射速（目前不确定是电控发还是自己设）
static const float shoot_speed = 1.0f;     //确保为米每秒
//炮口补偿系数X轴
static const float CompensationFactor_X = 0.0f;
//炮口补偿系数Y轴
//static const float CompensationFactor_Y = 7.0f;
static const float CompensationFactor_Y = 0.0f;
//炮口补偿系数Z轴
static const float CompensationFactor_Z = 14.5f;
//处理时间补偿
static const float CompensationFactor_Proc = 0.04f;
//重力加速度
static const float G = 980.0f;
//云台高度
static const float Height_PAN =  35;

//弧度
static const float Radian = 3.1415926/180;

//大装甲实际大小
static const float kRealLargeArmorWidth = 22.5;
static const float kRealLargeArmorHeight = 5.7;

//小装甲实际大小
static const float kRealSmallArmorWidth = 13.5;
static const float kRealSmallArmorHeight = 5.7;

//大幅装甲实际大小
static const float kRealRuneWidth = 24;
static const float kRealRuneHeight = 18;

static const int kOffsetX = 0;
static const int kOffsetY = 0;
static const int kOffsetZ = 0;



////usb相机内参---白色
//static const cv::Mat caremaMatrix_shoot = (
//        cv::Mat_<float>(3, 3) << 1813.46553187846,      0.180945448953958,               282.507136387746,
//                                 0,                     1816.19677142663,               210.369442077180,
//                                 0,                     0,                              1
//                                  );
//    //畸变参数
//static const cv::Mat distCoeffs_shoot = (
//        cv::Mat_<float>(1, 5) << 0.0979233792824180,
//                                 -11.9954986812870,
//                                 -0.00906339219523832,
//                                 -0.00739892895677977,
//                                 -71.2643298288405);

////daheng相机内参
//static const cv::Mat caremaMatrix_shoot = (
//        cv::Mat_<float>(3, 3) << 627.423411486903,      0.0428283181538756,             324.365147891400,
//                                 0,                     627.145798165945,               254.813183815399,
//                                 0,                     0,                              1
//                                  );
//    //畸变参数
//static const cv::Mat distCoeffs_shoot = (
//        cv::Mat_<float>(1, 5) << -0.277496398343736,
//                                 0.789238202332373,
//                                 -0.003856284737827252,
//                                 -0.00181822169733265,
//                                 -1.95707528678493);


////usb相机内参---黑色
//static const cv::Mat caremaMatrix_shoot = (
//        cv::Mat_<float>(3, 3) << 2730.92085752429,      9.41584716138016,              228.719557259776,
//                                 0,                     2726.09853503123,               224.153511883902,
//                                 0,                     0,                              1
//                                  );
//    //畸变参数
//static const cv::Mat distCoeffs_shoot = (
//        cv::Mat_<float>(1, 5) << 0.0974458165440733,
//                                 -3.27440194903448,
//                                 -0.00196575878203993,
//                                 -0.00223089667670566,
//                                 39.7565390804255);

//大恒相机2---黑色


// static const cv::Mat caremaMatrix_shoot = (
//        cv::Mat_<float>(3, 3) << 1267.90652664403,      -0.0353684014018146,           638.705398481063,
//                                 0,                     1268.69042858230,              503.442026283915,

//                                 0,                     0,                              1
//                                  );
//    //畸变参数
// static const cv::Mat distCoeffs_shoot = (
//        cv::Mat_<float>(1, 5) << -0.249322829161717,
//                                 0.515928586887545,
//                                 -0.00310212015615730,
//                                 -0.000444853745423472,
//                                 -1.09855488382837);



static const cv::Mat caremaMatrix_shoot = (
        cv::Mat_<float>(3, 3) << 1280.57004222812,      0.402061111965183,           639.220029499724,
                                 0,                     1279.32169906205,              511.592057672373,
                                 0,                     0,                              1
                                  );
    //畸变参数
static const cv::Mat distCoeffs_shoot = (
        cv::Mat_<float>(1, 5) << -0.220194070500902,
                                 0.254316617233456,
                                 -0.00362630692880902,
                                 -0.000451979898316082,
                                 -0.324934615116419);










/*
*   @brief:结构体
*/

}






#endif // CONSTANTS
