#include "predict.h"

namespace Horizon{

#define SIN_POINT_NUM 400
float SavePoint[SIN_POINT_NUM];         //保存点位
float SecSavePoint[SIN_POINT_NUM];      //保存点位
int Times = 0;

// int differential;                       //

/**
 * @brief  预测的整体控制框架
 * 
 * @author liqianqi
 * 
 * @return 返回云台要转到的绝对角度
 */

GimbalPose Predictor::predictLocation()
{
//    JudgeState();
//    gyroDector();
    GimbalPose return_gimbalpose;
    if(current_dector_state_ == DECTORSTATE::LOST)
    {
        cout << "current time is nonepredict" << endl;
        current_predict_mode_ = PREDICTORMODE::NONEPREDICT;
    }

    current_predict_mode_ = PREDICTORMODE::PREDICTOR;

    switch(current_predict_mode_)
    {
        case PREDICTORMODE::FOLLOW:
        {   
            // if(last_predict_mode_ != PREDICTORMODE::FOLLOW){
            //     initFollow();
            // }
            return_gimbalpose = followTarget();
            break;
        }
        case PREDICTORMODE::PREDICTOR:
        {
            // if(last_predict_mode_ != PREDICTORMODE::PREDICTOR){
            //     initPredict();
            // }
            return_gimbalpose = predictTarget();
            break;
        }
        case PREDICTORMODE::ANTIGYRO:
        {
            // if(last_predict_mode_ != PREDICTORMODE::ANTIGYRO){
            //     initAntiGyro();
            // }
            Vector3f point;
            point << best_target_.center3d_.x,best_target_.center3d_.y,best_target_.center3d_.z;
            point = cam3ptz(current_gimbalpose_,point);

            best_target_.center3d_.x = point[0];
            best_target_.center3d_.y = point[1];
            best_target_.center3d_.z = point[2];
            cout << return_gimbalpose << endl;
            return_gimbalpose = antiGyroTarget();
            break;
        }
        case PREDICTORMODE::NONEPREDICT :{
            return_gimbalpose.yaw = 0;
            return_gimbalpose.pitch = 0;
            break;
        }
    }
    //cout << "dsdd " << endl;
    //cout << "xxxxxxx" << return_gimbalpose.yaw << endl;
    return return_gimbalpose;
}
/**
 * @brief  跟随模式下的云台运动，用来检验视觉和电控收发数是否正确，以及检验卡尔曼滤波调整是否精准
 * 
 * @author liqianiqi
 * 
 * @return 返回当前云台所转过的位姿
 */

GimbalPose Predictor::followTarget()
{

    GimbalPose return_gim;
    if(is_init_)
    {
        last_target_ = best_target_;
        return_gim.pitch = 0;
        return_gim.yaw = 0;
        is_init_ = false;
        return return_gim;
    }
    Point3f getpoint;
    // 坐标系的解算
    Vector3f point;
    point[0] = best_target_.center3d_.x;
    point[1] = best_target_.center3d_.y;
    point[2] = best_target_.center3d_.z;
    //point = cam3ptz(current_gimbalpose_,point);
    getpoint.x = point[0];
    getpoint.y = point[1];
    getpoint.z = point[2];

    if(display_mode == DisplayMode::Open)
    {
        //CurveData::drawCurveData1(best_target_.center3d_);
    }

    return_gim.pitch = std::atan(((best_target_.center3d_.y)/sqrt(best_target_.center3d_.x * best_target_.center3d_.x + best_target_.center3d_.z * best_target_.center3d_.z))) * 180 / PI;
    return_gim.yaw = std::atan(best_target_.center3d_.x/best_target_.center3d_.z) * 180 / PI;


    if(isnan(return_gim.yaw))
    {
        return_gim.yaw = 0;
    }
    if(isnan(return_gim.pitch))
    {
        return_gim.pitch = 0;
    }

    if(display_mode == DisplayMode::Open)
    {
        Point3f a;
        a.x = return_gim.yaw;
        curvedata.drawCurveData(a);
    }

    return return_gim;
}

/**
 * @brief  预测模式下的自瞄状态
 * 
 * @author liqianqi
 * 
 * @return 返回当前云台转过的位姿
 */

GimbalPose Predictor::predictTarget()
{

    GimbalPose return_gim;
//    if(is_have_data_ && last_dector_state_ == DECTORSTATE::LOST)
//    {
//        current_dector_state_ = DECTORSTATE::CONTINUE;
//        last_dector_state_ = current_dector_state_;
//        is_init_ = true;
//    }
    if(is_init_){
        last_target_ = best_target_;
        //**计算实际坐标**//
        Vector3f point1;
        point1 << last_target_.center3d_.x,last_target_.center3d_.y,last_target_.center3d_.z;
        point1 = cam3ptz(current_gimbalpose_,point1);
        last_point_.x = point1[0];
        last_point_.y = point1[1];
        last_point_.z = point1[2];

        test_rate_point_ = last_point_;
        //**计算实际坐标**//
        
        return_gim.pitch = 0;
        return_gim.yaw = 0;
        is_init_ = false;
        last_target_.h_time_stamp_ = (int)now();
        cout << "predict initing" << endl;
        return return_gim;
    }
    Point3f getpoint;
    
//    if(!is_have_data_)
//    {
//        ++lost_of_image_;
//    }
//    else
//    {
//        lost_of_image_ = 0;
//    }

//    if(lost_of_image_ > 20)
//    {
//        current_dector_state_ = DECTORSTATE::LOST;
//        lost_of_image_ = 0;
//    }
//    if(current_dector_state_ == DECTORSTATE::LOST)
//    {
//        last_dector_state_ = current_dector_state_;
//        return gm_ptz;
//    }


    // 坐标系的解算
    Vector3f point;

    point << best_target_.center3d_.x,best_target_.center3d_.y,best_target_.center3d_.z;

    point = cam3ptz(current_gimbalpose_,point);

    visual_point3D_.x = point[0];
    visual_point3D_.y = point[1];
    visual_point3D_.z = point[2];

    getpoint.x = point[0];
    getpoint.y = point[1];
    getpoint.z = point[2];
    //getpoint = kalaman.KalmanMainRun(getpoint);

    cout << "the world corditions:  " << getpoint.x << "    " << getpoint.y << "    " << getpoint.z << endl;

    //float vx = (getpoint.x - last_point_.x)/(delta_t_);
    //float vy = (getpoint.y - last_point_.y)/(delta_t_);
    //float vz = (getpoint.z - last_point_.z)/(delta_t_);

    if(diffential_ == 15)
    {
        vx = (getpoint.x - test_rate_point_.x)/(15*delta_t_);
        vy = (getpoint.y - test_rate_point_.y)/(15*delta_t_);
        vz = (getpoint.z - test_rate_point_.z)/(15*delta_t_);
        test_rate_point_ = getpoint;
        diffential_ = 0;
    }
    ++diffential_;


    last_point_ = getpoint;

    Point3f landing_point;

    float t1 = bullteFlyTime(getpoint);

    Point3f speed_vector = Point3f(vx,vy,vz);

    speed_vector = kalaman.KalmanMainRun(speed_vector);

    last_speed_ = speed_vector;

    float a = speed_vector.x*(t1 + delta_t_ + 0.1);
    float b = speed_vector.y*(t1 + delta_t_ + 0.1);
    float c = speed_vector.z*(t1 + delta_t_ + 0.1);

    landing_point.x = getpoint.x + a;
    landing_point.z = getpoint.z + c;
    landing_point.y = getpoint.y + b;

    last_target_ = best_target_;

    t1_ = bullteFlyTime(landing_point);

    cout << "预测时间：  " << t1_ << endl;

    Point3f angle_kf = Point3f(gm_ptz.pitch,gm_ptz.yaw,0);
    //float gm_yaw = gm_ptz.yaw;
    //angle_kf = angle_kalaman.KPredictionRun(angle_kf);

    gm_ptz.pitch = angle_kf.x;
    gm_ptz.yaw = angle_kf.y;

    return_gim.pitch = gm_ptz.pitch;
    return_gim.yaw = gm_ptz.yaw;

    if(isnan(return_gim.yaw))
    {
        cout << "nan_yaw" << endl;
        return_gim.yaw = 0;
    }
    if(isnan(return_gim.pitch))
    {
        cout << "nan_pitch" << endl;
        return_gim.pitch = 0;
    }

    if(display_mode == DisplayMode::Open)
    {
        Point3f a;
        a.x = getpoint.x;
       // curvedata.drawCurveData(a);
        V_.x = speed_vector.x;
        V_.y = speed_vector.y;
        V_.z = speed_vector.z;
    }

    return return_gim;

}
/**
 * @brief  反陀螺模式下的自瞄状态
 * 
 * @author liqianqi
 * 
 * @return 返回当前云台要转过的绝对角度 
 */

GimbalPose Predictor::antiGyroTarget()
{
    if(last_predict_mode_ != PREDICTORMODE::ANTIGYRO){
        initAntiGyro();
    }
    // getArmor有问题
    last_target_ = best_target_;
    GimbalPose return_gimbalpose(0,0,0);
    if(select_data_of_antigyro_count_ < 30){
        antigyro.getArmor(best_target_);
        ++select_data_of_antigyro_count_;
    }

    if(select_data_of_antigyro_count_ == 30){
        is_select_antigyro_data_ = true;
    }

    if(is_select_antigyro_data_){
        return_gimbalpose = antigyro.getP_Y();
        two = antigyro.getPeriodTime();
        return return_gimbalpose;
    }
    if(isnan(return_gimbalpose.yaw)){
        return_gimbalpose.yaw = 0;
    }
    if(isnan(return_gimbalpose.pitch)){
        return_gimbalpose.pitch = 0;
    }
    return return_gimbalpose;
}
/**
 * @brief  判断当前的预测状态和识别状态
 * 
 * @author liqianqi
 */

void Predictor::JudgeState()
{
    static int buffer_count = 0;
    if(last_dector_state_ == DECTORSTATE::LOST && is_have_data_ == true)
    {
        current_dector_state_ = DECTORSTATE::FIRSTFIND;
        last_dector_state_ = current_dector_state_;
        initPredict();

    }else if(last_dector_state_ == DECTORSTATE::FIRSTFIND && is_have_data_ == true)
    {
        if(is_seem_armor(last_target_,best_target_))
        {
            current_dector_state_ = DECTORSTATE::CONTINUE;
            last_dector_state_ = current_dector_state_;
        }else
        {
            current_dector_state_ = DECTORSTATE::SWITCH;
            last_dector_state_ = current_dector_state_;
            if(last_predict_mode_ != PREDICTORMODE::ANTIGYRO)
            {
                initPredict();
            }

        }


    }
    else if(last_dector_state_ == DECTORSTATE::CONTINUE && is_have_data_ == true)
    {
        if(is_seem_armor(last_target_,best_target_))
        {
            current_dector_state_ = DECTORSTATE::CONTINUE;
            last_dector_state_ = current_dector_state_;
        }else
        {
            current_dector_state_ = DECTORSTATE::SWITCH;
            last_dector_state_ = current_dector_state_;
            if(last_predict_mode_ != PREDICTORMODE::ANTIGYRO)
            {
                initPredict();
            }
        }


    }else if(last_dector_state_ == DECTORSTATE::CONTINUE && is_have_data_ == false)
    {
        current_dector_state_ = DECTORSTATE::BUFFERING;
        last_dector_state_ = current_dector_state_;

    }else if(last_dector_state_ == DECTORSTATE::BUFFERING && is_have_data_ == false)
    {
        current_dector_state_ = DECTORSTATE::BUFFERING;
        last_dector_state_ = current_dector_state_;
        ++buffer_count;
        if(buffer_count > THRESHOLD::LOST_TARGET)
        {
            current_dector_state_ = DECTORSTATE::LOST;
            last_dector_state_ = current_dector_state_;
            buffer_count = 0;
            return ;
        }

    }else if(last_dector_state_ == DECTORSTATE::LOST && is_have_data_ == false)
    {
        current_dector_state_ = DECTORSTATE::LOST;
        last_dector_state_ = current_dector_state_;
        return ;

    }else if(last_dector_state_ == DECTORSTATE::SWITCH && is_have_data_ == true)
    {
        if(is_seem_armor(last_target_,best_target_))
        {
            current_dector_state_ = DECTORSTATE::CONTINUE;
            last_dector_state_ = current_dector_state_;
        }else
        {
            current_dector_state_ = DECTORSTATE::SWITCH;
            last_dector_state_ = current_dector_state_;
            if(last_predict_mode_ != PREDICTORMODE::ANTIGYRO)
            {
                initPredict();
            }
        }

    }else if(last_dector_state_ == DECTORSTATE::SWITCH && is_have_data_ == false)
    {
        current_dector_state_ = DECTORSTATE::BUFFERING;
        last_dector_state_ = current_dector_state_;
    }

}
/**
 * @brief  判断是否为反陀螺状态，在之后可以加上方向上的判断
 * 
 * @author liqianqi
 * 
 * @return 返回是否为反陀螺状态
 */

bool Predictor::gyroDector()
{
    static int anti_count = 0;
    if(current_dector_state_ == DECTORSTATE::LOST||current_dector_state_ == DECTORSTATE::BUFFERING)
    {
        anti_count = 0;
        return false;
    }
    if(!is_seem_armor(last_target_,best_target_))
    {
        current_dector_state_ = DECTORSTATE::SWITCH;
        last_dector_state_ = current_dector_state_;
        if(anti_count == 0)
        {
            last_switch_time_ = best_target_.h_time_stamp_;
            ++anti_count;
        }else if(best_target_.h_time_stamp_ - last_switch_time_ < 1.0)
        {
            last_switch_time_ = best_target_.h_time_stamp_;
            ++anti_count;
            if(anti_count >= 6)
            {
                is_antigyro_ = true;
                current_predict_mode_ = PREDICTORMODE::ANTIGYRO;
                cout << "/*is   ******************   antigyro*/" << endl;
                last_predict_mode_ = current_predict_mode_;
                anti_count = 0;
                return true;
            }
            
        }else
        {
            
        }

    }else
    {
        if(last_predict_mode_ == PREDICTORMODE::ANTIGYRO){
            // last_switch_time_ = best_target_.h_time_stamp_;
            if(best_target_.h_time_stamp_ - last_switch_time_ > 1.0)
            {
                current_dector_state_ = DECTORSTATE::CONTINUE;
                current_predict_mode_ = PREDICTORMODE::PREDICTOR;
                last_predict_mode_ = current_predict_mode_;
                last_dector_state_ = current_dector_state_;
                return false;
            }
            return true;
        }else
        {
            
        }
        // current_dector_state_ = DECTORSTATE::CONTINUE;
    }
    return false;

}
/**
 * @brief  判断前后两帧得到的装甲板是否为同一个
 * 
 * @author liqianqi
 * 
 * @param a1：装甲版1
 * @param a2：装甲板2
 * 
 * @return 返回是否为同一个装甲板
 */
bool Predictor::is_seem_armor(const Armor &a1, const Armor &a2)
{
    if(getDistance(a1.center3d_,a2.center3d_)>THRESHOLD::SEEM_ARMOR_MIN_SPACE_DISTANCE)return false;
    if(getDistance(a1.center2d_,a2.center2d_)>THRESHOLD::SEEM_ARMOR_MIN_PIEX_DISTANCE)return false;
    return true;
}
/**
 * @brief 判断是否要对预测模式初始化
 * 
 */
void Predictor::initPredict()
{
    is_init_ = true;
    
}
/**
 * @brief 判断是否要对反陀螺自瞄初始化
 * 
 */
void Predictor::initAntiGyro()
{
    select_data_of_antigyro_count_ = 0;
}
/**
 * @brief  判断弹道的飞行时间
 * 
 * @param  三维坐标******************************************************************************************************************************************************************8
 * 
 * @return 飞行的时间
 */

float Predictor::bullteFlyTime(Point3f p1)
{   
    // mm->m
    p1.x = p1.x / 1000;
    p1.y = p1.y / 1000;
    p1.z = p1.z / 1000;

    float v0_ = v0;

    cout << "bullet_speed:  " << v0_ << endl;
    
    float g = 9.80665;

    // cout << "******" << p1.x<< "*******" << p1.y << "*******" << p1.z << endl;

    // cout << "******" << "*******" << v0_ << endl;

    //float x1,y1,z1;
    //先算yaw的值的
    float distance1;
    distance1 = std::sqrt(p1.x*p1.x+p1.z*p1.z);
    gm_ptz.yaw = std::asin(p1.x/distance1)*180/CV_PI;

//    if(p1.z<0&&p1.x>=0){
//        gm_ptz.yaw = 2*(90 - gm_ptz.yaw) + gm_ptz.yaw;
//    }
//    else if(p1.z<0&&p1.x<0)
//    {
//        gm_ptz.yaw = 2*(-90 - gm_ptz.yaw) + gm_ptz.yaw;
//    }

    //pitch值,这里的问题。。。
    float a = -0.5*g*(std::pow(distance1,2)/std::pow(v0_,2));
    float b = distance1;
    float c = a - p1.y;
    
    //cout << "   " << a << "    " << b << "    " << c << endl;

    float Discriminant = std::pow(b,2) - 4*a*c;  //判别式
    if(Discriminant < 0) return -1;
    float tan_angle1 = (-b + std::pow(Discriminant,0.5))/(2*a);//*180/CV_PI;
    float tan_angle2 = (-b - std::pow(Discriminant,0.5))/(2*a);

    float angle1 = std::atan(tan_angle1)*180/CV_PI;
    float angle2 = std::atan(tan_angle2)*180/CV_PI;

	if (tan_angle1 >= -3 && tan_angle1 <= 3) 
    {   
        cout << "angle1     " << angle1 << endl;
        gm_ptz.pitch = angle1;
	}
	else if (tan_angle2 >= -3 && tan_angle2 <= 3) 
    {
        cout << "angle2     " << angle2 << endl;
        gm_ptz.pitch = angle2;
	}
    //cout << "角度是：   " << tan_angle1  << "   " << tan_angle2 << endl;
    float PI_pitch = (gm_ptz.pitch/180)*CV_PI;
    //cout << v0_ << endl;

    return distance1/(v0_*std::cos(PI_pitch));

}
/**
 * @brief  迭代法得到当前弹道最好的落点
 * @author 桂林电子科技大学Evolution战队
 * @param coordinate_m 当前得到具体的三维坐标（卡尔曼滤波后）
 * @param shootSpeed  子弹初速度
 * @param vx 子弹沿着x轴的速度
 * @param vz 子弹沿着y轴的速度
 * @return 返回弹道最好的落点
 */

cv::Point3f Predictor::Iteration(cv::Point3f coordinate_m, float shootSpeed,float vx, float vz)
{

    shootSpeed = shootSpeed/1000;
	float tof, TOF;                                  //弹丸飞行时间，由迭代得到
	float d = sqrtf(coordinate_m.x * coordinate_m.x + coordinate_m.z * coordinate_m.z);		//无预判情况下的水平距离

	cv::Point3f tmpcoord;
	tmpcoord.x = coordinate_m.x;
	tmpcoord.y = coordinate_m.y;
	tmpcoord.z = coordinate_m.z;


    cout << "$$$$$$$$$$$$$$$$$$$$    " << vx << endl;

	float beta = angleCalculation(tmpcoord, shootSpeed);   //pitch轴角度
	//cout << "beta: " << beta << endl;
	float v = cosf(ANGLE_TO_RADIAN(beta)) * shootSpeed;    //速度的水平分量
	//cout << "v: " << v << endl;
	TOF = d / v;   //得到无预判情况下的飞行时间
	//cout << "shootSpeed: " << shootSpeed;
	//cout << "distance d: " << d << endl;
	//cout << "no pre TOF" << TOF << endl;

	//单位为m
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float temp = 1;
	int cnt = 0;
//	开始迭代
//  当时间差小于10的5次方且迭代次数小于15
//  第一次迭代T为无预判飞行时间

    while (fabs(temp) > 1e-5 && cnt < 20)
    {

        // x = coordinate_m.x + _V.x * TOF + _A.x * TOF * TOF / 2.0f;
        // y = coordinate_m.y + _V.y * TOF + _A.y * TOF * TOF / 2.0f;
        // z = coordinate_m.z + _V.z * TOF + _A.z * TOF * TOF / 2.0f;

        x = coordinate_m.x + vx * TOF;
        y = coordinate_m.y + vz * TOF;
        y = coordinate_m.y;
        //z = coordinate_m.z + _V.z * TOF;

		//预测位置的水平距离
		float D = sqrtf(x * x + z * z);
		//cout << "D: " << D << endl;
		//得到预测飞行时间
		cv::Point3f coord;
		coord.x = x;
		coord.y = y;
		coord.z = z;
		//参与运算的单位为m

		beta = angleCalculation(coord, shootSpeed);     //获得迭代中pitch轴角度

        //cout << "beta: " << beta << endl;
		v = shootSpeed * cosf(ANGLE_TO_RADIAN(beta));
		if (v > 0) {
			tof = D / v;  //预测的飞行时间
		}
		else {
			tof = TOF;
		}
		//获得时间差
		temp = TOF - tof;
		//迭代余项 预判点的距离与最后一次观测点的差值
		float delta = fabs(temp);
        /*
		cout << "TOF: " << TOF << endl;
		cout << "tof: " << tof << endl;
		cout << "temp: " << temp << endl;
		cout << endl;
        */
		//向期望点收敛
		if (temp > 0) {
			TOF -= delta;
		}
		else {
			TOF += delta;
		}
		cnt++;
	}

    if (cnt >= 20) {
		cout << "prediction interation fail!" << endl;
	}
    if(current_dector_state_ == DECTORSTATE::CONTINUE || current_dector_state_ == DECTORSTATE::FIRSTFIND){
        _t = tof;
    }

	/*
	cout << " origin x: " << coordinate_m.x << endl;
	cout << " origin y: " << coordinate_m.y << endl;
	cout << " origin z: " << coordinate_m.z << endl;
	cout << "cnt: " << cnt << endl;
	cout << "final TOF: " << tof << endl;
	cout << " x: " << M_TO_MM_TRANS(x);
	cout << " y: " << M_TO_MM_TRANS(y);
	cout << " z: " << M_TO_MM_TRANS(z) << endl << endl;
	*/

	return cv::Point3f(x, y, z);
}
/**
 * @brief 返回pitch
 * @param coordinate 点对应的pitch值
 * @param shootSpeed 射速
 * @return pitch值
 */

float Predictor::angleCalculation(const cv::Point3f &coordinate, const float &shootSpeed)
{

	//相机坐标系旋转，使其zox平面水平,y轴垂直 
	float x = coordinate.x;
	float z = coordinate.z;
    float y = coordinate.y;
	float speed = shootSpeed;
	float tanAngleA = 0.0f;
	float tanAngleB = 0.0f;
	
	float p = sqrtf(x * x + z * z);     //获取水平距离
	float pitchAngleRef = 0.0f;

	/*
	cout << "horizonPosition = " << p << endl;
	cout << "verticalPosition = " << y << endl;
	*/

	float a = -GRAVITY * p * p / (2 * speed * speed);
	float b = p;
	float c = (-y + a);
	float delta = b * b - 4 * a * c;
	float mid = -b / (2 * a);
	/*
        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
        cout << "c = " << c << endl;
        cout << "delta = " << delta << endl;
        cout << "mid = " << mid << endl;
	*/


	if (delta < 0)
    {
        pitchAngleRef = (atanf(y / p));
	}
	else if (delta == 0) 
    {
        tanAngleA = mid;
        pitchAngleRef = atanf(tanAngleA);
		//cout << "pitchAngleRef = " << pitchAngleRef << endl;
	}
	else if (delta > 0) 
    {


		tanAngleA = mid - sqrtf(delta) / (2 * a);
		tanAngleB = mid + sqrtf(delta) / (2 * a);

		if (tanAngleA >= -1 && tanAngleA <= 1) 
        {
            pitchAngleRef = atanf(tanAngleA);
		}
		else if (tanAngleB >= -1 && tanAngleB <= 1) 
        {
            pitchAngleRef = atanf(tanAngleB);

		}

		//       cout << "tanAngleA = " << tanAngleA << endl;
		//       cout << "tanAngleB = " << tanAngleB << endl;
		//       cout << "pitchAngleRef = " << pitchAngleRef << endl;
        
	}
	return pitchAngleRef;
}

/*****************绘制曲线的部分*****************/



bool CurveData::isEnable() {
    return instance().CURVE_SWITCH;
}


void CurveData::clear() 
{

    instance()._nowTime_ms.clear();
    instance()._coordinateOrigin.clear();
    instance()._coordinatePredict.clear();
    instance()._coordinateFilter.clear();
    instance()._velocityOrigin.clear();
    instance()._velocityFilter.clear();
    instance().accOrigin.clear();
    instance()._angleNoBias.clear();
    instance()._angleRef.clear();
    instance()._angleFbd.clear();
    instance()._angleTime.clear();
    instance()._isEmpty = true;
};
/**
 * @author : 桂林电子科技大学Evolution战队
 *
 * @brief  : 将绘制的数据写入文件
 */

/**
 * @author : 华北理工大学Horizon战队
 *
 * @brief  : 将数据以波形图的方法绘制出来，更好观察
 */
void CurveData::drawCurveData(cv::Point3f point){

    Mat poly_background_src_ = cv::Mat::zeros(1024, 1280, CV_8UC3);
    poly_background_src_.setTo(cv::Scalar(0, 255, 0));

    SavePoint[Times%SIN_POINT_NUM] = point.x;
    // 找最大值的方法只用循环一次就可以
    float maxData = 0;
    for(int i = 0;i<=Times&&i<SIN_POINT_NUM;i++){
        if(fabs(SavePoint[i])>maxData){
            maxData = fabs(SavePoint[i]);
        }
    }
    // 计算倍率
    float bei = 10;
    if(10*maxData > poly_background_src_.rows/2){
        bei = (poly_background_src_.rows/2 - 10)/maxData;
    }
    if(10*maxData<150 && maxData!=0){
        bei = 150.0/maxData;
    }
    std::vector<cv::Point> points;
    // 存入当前记录位置的后续单位,靠前的位置
    int t = 0;
    for(int i = (Times + 1)%SIN_POINT_NUM;i<=Times&&i<SIN_POINT_NUM;i++){
        points.push_back(Point((float)poly_background_src_.cols/SIN_POINT_NUM*t ,poly_background_src_.rows/2+bei*SavePoint[i]));
        t++;
    }
    // 绘制折线
    //cv::polylines(poly_background_src_, points, false, cv::Scalar(255, 0, 0), 1, 8, 0);
    //存入之前的元素
    for(int i = 0;i<=Times %SIN_POINT_NUM;i++){
        points.push_back(Point( (float)poly_background_src_.cols/SIN_POINT_NUM*t ,poly_background_src_.rows/2+bei*SavePoint[i]));
        t++;
    }

    cv::polylines(poly_background_src_, points, false, cv::Scalar(0, 0, 255), 3, 8, 0);

        //cv::polylines(poly_background_src_, points, false, cv::Scalar(255, 0, 0), 3, 8, 0);

    string windowName = "波形图-预测前";
    namedWindow(windowName, 0);
    imshow(windowName,poly_background_src_);
    Times++;

}



bool CurveData::isEmpty(){
    return instance()._isEmpty;
}


}
