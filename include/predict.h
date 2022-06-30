#include "common.h"
//#include "kalaman.h"
#include "antigyro.h"

namespace Horizon{

/**
 * 欠缺：
 * 1. 初始化构造函数有bug
 * 2. kalaman
 * 3. 反陀螺没有测试过
 * 4. 迭代的作用是否明显，调参是否容易，未知
 * 5. 辅助功能还未加，波形图(注：波形图是直接用自己的)
*/
#define CURVE_DATA_PATH "../visionData/curve/"
#define MM_TO_M_TRANS(p)               (float)(p) / 1000.0f
#define M_TO_MM_TRANS(p)	            (float)(p) * 1000.0f

#define ANGLE_TO_RADIAN(p)                  (p) * (float)CV_PI / 180.0f
#define RADIAN_TO_ANGLE(p)                  (p) * 180.0f / (float)CV_PI

#define GRAVITY			9.80565f	// m/s^2
#define PI              3.14159265358979f
#define TINY            1.0e-10f     //为防止0做分母，引入极小项

enum class PREDICTORMODE{
    FOLLOW,
    PREDICTOR,
    ANTIGYRO,
    NONEPREDICT
};
enum THRESHOLD{
    LOST_TARGET = 10,
    SEEM_ARMOR_MIN_SPACE_DISTANCE = 30,     //相同装甲前后两帧相差的可允许最小空间距离差
    SEEM_ARMOR_MIN_PIEX_DISTANCE = 30       //相同装甲前后两帧相差的可允许最小像素距离差
};


//曲线数据
class CurveData {
private:
    static CurveData& instance() {
        static CurveData curveData;
        return curveData;
    }

    const bool CURVE_SWITCH = true;				//曲线绘制开关
    std::vector<float> _nowTime_ms;					//存储坐标点对应时间
    std::vector<cv::Point3f> _velocityOrigin;		//存储坐标点速度
    std::vector<cv::Point3f> _velocityFilter;		//存储坐标点速度
    std::vector<cv::Point3f> accOrigin;			//存储加速度数据
    std::vector<cv::Point3f> _coordinateOrigin;		//存储原始坐标点数据
    std::vector<cv::Point3f> _coordinatePredict;	//存储预判坐标点数据
    std::vector<cv::Point3f> _coordinateFilter;		//存储滤波坐标点数据
    cv::Point3f _coordinateNoBias;		//无补偿量时的预判坐标点数据
    std::vector<cv::Point2f> _angleNoBias;
    std::vector<cv::Point2f> _angleRef;
    std::vector<cv::Point2f> _angleFbd;
    std::vector<float>  _angleTime;
    int _writeCnt = 0;
    const int MAX_WRITE_CNT = 300;
    bool _isEmpty = true;
    //Mat poly_background_img_;
    bool is_red_;
public:

    /**
     * 保存坐标点
     * @param point	水平坐标点（不要被xyz的顺序迷惑）
     *
     */
    static void saveTime(float);
    static void saveVelocity(cv::Point3f origin, cv::Point3f Filter);
    static void saveAcc(cv::Point3f origin);
    static void saveCoordinate( cv::Point3f origin, cv::Point3f filter, cv::Point3f prediction);
    static void saveNoBiasCoordinate(cv::Point3f coordinate);
    static cv::Point3f getNoBiasCoordinate();
    static void saveAngle(cv::Point2f angleNoBias, cv::Point2f angleRef, cv::Point2f angleFbd);

    /**
     * 清空坐标
     */
    static void clear();

    /**
    * 写入曲线文件
    * 每一行为一组数据，不同轴上的数据用空格隔开
    */
    static void write();
    /**
    * 实时绘制曲线
    * polylines()绘制曲线文件
    */
    void drawCurveData(cv::Point3f point);

    /**
    * 数据是否为空
    */
    static bool isEmpty();

    /**
    * 是否启用曲线绘制
    */
    static bool isEnable();

};

class Predictor{
public:
    Predictor():last_dector_state_(DECTORSTATE::LOST),
        last_predict_mode_(PREDICTORMODE::NONEPREDICT),
        is_have_data_(false),
        is_antigyro_(false),
        is_init_(true),
        is_select_antigyro_data_(false),
        select_data_of_antigyro_count_(0)
    {
        last_point_.x = 0;
        last_point_.y = 0;
        last_point_.z = 0;
    };
    ~Predictor(){}
    float v0;
    //Predictor();

    GimbalPose predictLocation();           // 预测位置，返回云台要转的角度

    GimbalPose followTarget();              // 跟随模式
    GimbalPose predictTarget();             // 预测模式
    GimbalPose antiGyroTarget();            // 反陀螺模式
    void JudgeState();                      // 判断状态
    bool is_seem_armor(const Armor &a1, const Armor &a2);// 是否相同装甲板或者是否连续
    bool gyroDector();                      // 陀螺检测，包含什么时候进入陀螺，什么时候退出陀螺
    bool gyroDectorSecond();                // 陀螺检测第二版，包含什么时候进陀螺，什么时候退陀螺

public:
    void initPredict();                     // 初始化预测参数
    void initFollow();                      // 初始化跟随参数
    void initAntiGyro();                    // 初始化反陀螺参数
	cv::Point3f Iteration(cv::Point3f coordinate_m, float shootSpeed,float vx, float vz); // 迭代法求解
    float angleCalculation(const cv::Point3f &coordinate, const float &shootSpeed);       // 求解pitch

public:
    // 获得电控发过来的数据
    void getPrivateData(float &v0, Armor &armor, GimbalPose &current_gimbalpose,bool is_have_data,float delta_t){
        //v0 = 0.5;
        this->v0 = v0;
        this->current_gimbalpose_ = current_gimbalpose;
        this->best_target_ = armor;
        this->is_have_data_ = is_have_data;
        this->best_target_.h_time_stamp_ = now();
        this->delta_t_ = delta_t;
    }
    // 将反陀螺的数据传除去
    std::tuple<long,long> returnAntiGyroData(){
        return two;
    }
    // 传出去是否为反陀螺
    bool antiPamam(){
        return is_antigyro_;
    }

public:
    float bullteFlyTime(Point3f p1);
private:
    /// 状态的变量
    DECTORSTATE last_dector_state_;         // 上一帧识别的状态
    PREDICTORMODE last_predict_mode_;       // 上一帧预测模式
    DECTORSTATE  current_dector_state_;     // 这一帧识别的状态
    PREDICTORMODE current_predict_mode_;    // 这一帧预测状态
    bool is_have_data_;                     // 是否有数据
    bool is_antigyro_;                      // 是否是反陀螺
    bool is_init_;                          // 此帧是否是初始化的

private:
    /// 测量数据变量
    Armor last_target_;                     // 上一帧目标
    vector<Armor> current_armor_seq_;       // 当前装甲板序列
    Armor best_target_;                     // 最好的装甲板

private:
    GimbalPose gm_ptz;                      // 计算好的弹道落点

private:
    /// 收集到的变量
    GimbalPose current_gimbalpose_;         // 这一帧对应的当前位姿
    Point3f last_point_;                    // 上一帧对应的坐标（云台下的）

private:
    /// 一些状态变量
    Kalman_Filter kalaman;
    //Kalman_Filter angle_kalaman;
    AntiGyro antigyro;
    tuple<long ,long> two;
    long last_switch_time_;                 // 上一次切换的时间
    bool is_select_antigyro_data_;          // 是否收集好了陀螺数据

private:
    /// 计数变量
    int select_data_of_antigyro_count_;     // 收集反陀螺数据
    /// 记录丢帧前的变量
    Point3f last_speed_;
    int lost_of_image_;

    /// 记录丢帧前的子弹飞行时间，由于缓冲区的时间很短，这个时间可以作为近似
    float _t;
    /// 辅助功能相关变量
    Mat poly_img_ = Mat(Size(640, 480), CV_8UC3, Scalar(0, 255, 0));

    CurveData curvedata;

    //enum DisplayMode {Open = 1,Close = 0};

public:
    // 用于可视化的数据
    Point3f V_;
    float t1_;
    Point3f visual_point3D_;
    float delta_t_;


    int diffential_ = 0;
    Point3f test_rate_point_;
    float vx = 0;
    float vy = 0;
    float vz = 0;

    /*
    * @brief:  相机坐标系转云台坐标系
    *
    * @parma:  gm是当前云台的偏转角，pos是要转换到云台的坐标系
    *
    * @author: 李黔琦
    *
    */
    Vector3f cam3ptz(GimbalPose &gm,Vector3f &pos){
        pos = pos.transpose();

        Matrix3f pitch_rotation_matrix_;
        Matrix3f yaw_rotation_matrix_;

        gm.pitch = (gm.pitch*CV_PI)/180;
        gm.yaw = (gm.yaw*CV_PI)/180;

        pitch_rotation_matrix_
        <<
        1,              0,              0,
        0,  std::cos(gm.pitch),  std::sin(gm.pitch),
        0,  -std::sin(gm.pitch), std::cos(gm.pitch);

        yaw_rotation_matrix_
        <<
        std::cos(gm.yaw),  0,  std::sin(gm.yaw),
        0,                 1,            0,
        -std::sin(gm.yaw), 0,  std::cos(gm.yaw);

        Vector3f t_pos_ = yaw_rotation_matrix_ * pitch_rotation_matrix_ * pos;
        return t_pos_;
    }

};



}


