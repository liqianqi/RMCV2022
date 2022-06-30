#include "antigyro.h"

namespace Horizon{


void AntiGyro::initData(){
    armors.clear();
}
void AntiGyro::getArmor(Armor &armor){
//    kalaman.predict(armor.center3d_.x);
//    kalaman.predict(armor.center3d_.y);
//    kalaman.predict(armor.center3d_.z);
    //kalaman.KalmanMainRun(armor.center3d_);
    armors.emplace_back(armor);
    armors.reserve(60);
}

float AntiGyro::bullteFlyTime(Armor &armor_fix){

    Vector3f pos;
    pos << armor_fix.center3d_.x,armor_fix.center3d_.y,armor_fix.center3d_.z;
    float g = 9.80665;

//    float x1,y1,z1;

    //先算yaw的值的
    float distance1;
    distance1 = sqrt(pos[0]*pos[0]+pos[2]*pos[2]);
    return_gimbalpose.yaw = asin(pos[0]/distance1)*180/CV_PI;

    //pitch值,这里的问题
    float a = -0.5*g*(pow(distance1,2)/pow(v0,2));
    float b = distance1;
    float c = -((0.5*g*pow(distance1,2))/pow(v0,2) + pos[1]);

    float Discriminant = pow(b,2) - 4*a*c;  //判别式
    if(Discriminant < 0) return -1;
    float tan_angle1 = (-b + pow(Discriminant,0.5)/(2*a));//*180/CV_PI;
    float tan_angle2 = (-b - pow(Discriminant,0.5)/(2*a));

    float angle1 = atan(tan_angle1)*180/CV_PI;
    float angle2 = atan(tan_angle2)*180/CV_PI;

    if(fabs(angle1) <= fabs(angle2) && fabs(angle1) < 45){
        return_gimbalpose.pitch = angle1;
        cout << angle1;
    }else if(fabs(angle2) < 45){
        return_gimbalpose.pitch = angle2;
        cout << angle2;
    }else{
        //cout << "计算解不符合实际" << endl;
        return -1;
    }

    return distance1/(v0*sin(return_gimbalpose.pitch));

}

GimbalPose AntiGyro::getP_Y(){

    /* 转绝对坐标，有一个数接收,这个转一下绝对坐标
    *
    *
    *
   */
    float floatX[armors.size()];
    float floatY[armors.size()];
    float floatZ[armors.size()];
    //int a[10];
    for(size_t i = 0;i < armors.size();i++){
        floatX[i] = armors[i].center3d_.x;
        floatY[i] = armors[i].center3d_.y;
        floatZ[i] = armors[i].center3d_.z;
    }
    sort(floatX,floatX + armors.size());
    sort(floatY,floatY + armors.size());
    sort(floatZ,floatZ + armors.size());

    float x_fit = (floatX[0] + floatX[armors.size()-1])/2;
    float y_fit = (floatY[0] + floatY[armors.size()-1])/2;
    float z_fit = (floatZ[0] + floatZ[armors.size()-1])/2;

    Armor armor_fix;
    armor_fix.center3d_.x = x_fit;
    armor_fix.center3d_.y = y_fit;
    armor_fix.center3d_.z = z_fit;

    bullettime = bullteFlyTime(armor_fix);

    return return_gimbalpose;

}


std::tuple<float,float> AntiGyro::getPeriodTime(){
    vector<Armor> list(armors);

    Armor armor_fix = smoothDataFilter1(list);
    
    getPalance();
    //cout << "1111" << endl;
    start_time_ = armor_fix.h_time_stamp_ - bullettime;

    frequence_ = CV_PI/(2*w_rad_);
    std::tuple<float,float> two = make_tuple(start_time_,frequence_);
    return two;
}

Armor AntiGyro::smoothDataFilter1(vector<Armor> &list){
    int l = 0;
    for(size_t k = 0;k < list.size();k++){
        for(size_t m = 0;m < list.size()-1-k;m++){
            list[m].distance_ = sqrt((list[m].center3d_.x*list[m].center3d_.x)+(list[m].center3d_.y*list[m].center3d_.y)+(list[m].center3d_.z*list[m].center3d_.z));
            if(list[m].distance_ > list[m+1].distance_){
                l = m;
            }else{
                l = m+1;
            }
        }
    }
    return list[l];
}

void AntiGyro::getPalance(){
    vector<Armor> armor_list = armors;
    for(size_t i = 0;i < armor_list.size();i++){
        for(size_t j = 0;j < armor_list.size()-1-i;j++){
            if(armor_list[j].center2d_.x < armor_list[j+1].center2d_.x){
                float temp;
                temp = armor_list[j].center2d_.x;
                armor_list[j].center2d_.x = armor_list[j+1].center2d_.x;
                armor_list[j+1].center2d_.x = temp;
            }
        }
    }

    // 拟合直线公式 y = k * x + b ，其中 y 为 angle，x 为 次数，k 为 palstance
    double avg_x  = 0;
    double avg_x2 = 0;
    double avg_y  = 0;
    double avg_xy = 0;

    double first = 0;
    for (unsigned long k = 0; k < armor_list.size(); k++)
    {
        avg_x  += k - first;
        avg_x2 += pow(k - first, 2);
        avg_y  += armor_list[k].center2d_.x;
        avg_xy += (k - first) * armor_list[k].center2d_.x;
    }
    avg_x  /= armor_list.size();
    avg_x2 /= armor_list.size();
    avg_y  /= armor_list.size();
    avg_xy /= armor_list.size();
    w_rad_ = (avg_xy - avg_x * avg_y) / (avg_x2 - pow(avg_x, 2));
    //armor_list.clear();
   // cout << "hello" << endl;

}

bool AntiGyro::DectorIsGyro(){

    vector<float> yaw_angle;
    yaw_angle.clear();

    for(size_t i = 0;i < armors.size();i++){

        float gyro_angle = atan((armors[i].center3d_.x*armors[i].center3d_.x)/(armors[i].center3d_.y*armors[i].center3d_.y));
        // 这里为什么出错
        yaw_angle.push_back(gyro_angle);

    }

    for(size_t i = 0;i < yaw_angle.size();i++){
        for(size_t j = 0;j < yaw_angle.size()-1-i;j++){
            if(yaw_angle[j] > yaw_angle[j+1]){
                float temp;
                temp = yaw_angle[j];
                yaw_angle[j] = yaw_angle[j+1];
                yaw_angle[j+1] = temp;
            }
        }
    }
    double avg_x  = 0;
    double avg_x2 = 0;
    double avg_y  = 0;
    double avg_xy = 0;

    double first = 0;
    for (unsigned long k = 0; k < yaw_angle.size(); k++)
    {
        avg_x  += k - first;
        avg_x2 += pow(k - first, 2);
        avg_y  += yaw_angle[k];
        avg_xy += (k - first) * yaw_angle[k];
    }
    avg_x  /= yaw_angle.size();
    avg_x2 /= yaw_angle.size();
    avg_y  /= yaw_angle.size();
    avg_xy /= yaw_angle.size();
    float palance = (avg_xy - avg_x * avg_y) / (avg_x2 - pow(avg_x, 2));

    if(palance > MIN_W_RAD){
        return true;
    }

    return false;
}
/// 第二种方法写进预测部分里面了




}
