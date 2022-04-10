#ifndef ANTIGYRO_H
#define ANTIGYRO_H
#include "common.h"
#include "kalaman.h"
namespace Horizon{

#define MIN_W_RAD  CV_PI/6
class AntiGyro{
public:
    AntiGyro(){}
    ~AntiGyro(){}
    bool DectorIsGyro();

    bool DectorIsGyroSecond();// 第二判断方式
    void initData();
    void getArmor(Armor &armor);
    //void getPalance();

    float bullteFlyTime(Armor &armor_fix);
    GimbalPose getP_Y();
    Armor smoothDataFilter1(vector<Armor> &list);
    std::tuple<float,float> getPeriodTime();
    void getPalance();
    void clear(){
        armors.clear();
        start_time_ = 0;
        frequence_ = 0;
        w_rad_ = 0;
        //armor_list.clear();
    }
private:
    float v0 = 10.0;
    //vector<Armor> armor_list;
    vector<Armor> armors;
    GimbalPose return_gimbalpose;

    float bullettime;
    float start_time_;
    float frequence_;
    float w_rad_;
    Kalman_Filter kalaman;
};


}


#endif // ANTIGYRO_/H
