#include "common.h"
#include "predict.h"
#include "Send_Receive.h"
#include "ArmorDector.h"
#include "measure.h"
#include <mutex>
#include <chrono>
#include "VideoCapture.h"
using namespace myown;
//#define CAMERAMODE USB

<<<<<<< HEAD
#define USB_CAMERA_PATH "/dev/video0"
=======
#define USB_CAMERA_PATH "/dev/video2"
>>>>>>> 这一版增加了数字识别，但效果不太理想
enum class SerialState
{
    SEND,
    RECEIVE,
    WAIT
};

<<<<<<< HEAD
enum BufferSize{
    IMGAE_BUFFER = 5
};

class Rmvision{

public:
    // 这个地方先标注出来，有点大问题
    Rmvision():video_format_width(640),video_format_height(480),usb_exp_vule(82){};// 声明为默认构造函数
=======
enum BufferSize
{
    IMGAE_BUFFER = 5
};

class Rmvision
{

public:
    // 这个地方先标注出来，有点大问题
    Rmvision():video_format_width(640),video_format_height(480),usb_exp_vule(82),is_armor_(false){};// 声明为默认构造函数
>>>>>>> 这一版增加了数字识别，但效果不太理想
    ~Rmvision(){}
    void Fronted();
    void Backend();
    void Sent_Receive();
    // 两个相机的来回切换
    void DaHengCameraProducer();
    void USBCameraProducer();

private:
    // 前端
    ArmorDector armordector;
    // 后端和发数控制器
    Horizon::Predictor predictor;
    Horizon::DataControler datacontroler;
    // 测量距离
    MeasureDistance measuredistance;
    // 电控和视觉串口发数和收数，要发的数和要收的数
    Horizon::DataControler::VisionData visiondata;  // 视觉向电控传数据
    Horizon::DataControler::Stm32Data stm32data;    // 电控向视觉发数据
    Armor armor;                                    // 得到的最好的装甲板

private:
<<<<<<< HEAD
    volatile SerialState serial_state_;//收发数的状态
    mutex serial_mutex_;               //数据上🔓
=======
    volatile SerialState serial_state_;// 收发数的状态
    mutex serial_mutex_;               // 数据上🔓
>>>>>>> 这一版增加了数字识别，但效果不太理想

private:
    // 陀螺周期和发弹频率，仅在陀螺模式下使用
    std::tuple<long,long> T_f;
    // 定义收数位姿
    GimbalPose gimbal;
    // 定义发数的位姿
    GimbalPose gim;
    //enum DisplayMode {Open = 1,Close = 0};
private:
    // 生产者消费者模式
    Mat image_buffer_[IMGAE_BUFFER];
    volatile unsigned int image_buffer_front_ = 0;
    volatile unsigned int image_buffer_rear_ = 0;
private:
    long t1;
    long t2;
    long start_fire;
    bool is_armor_;
    unsigned short armor_number_;
private:
<<<<<<< HEAD
    double FPS;
=======
    double FPS;                         // 获取图像帧率
    double GET_IMAGE_FREQUENCE_;        // 抓取图像帧率
>>>>>>> 这一版增加了数字识别，但效果不太理想

    int usb_exp_vule;
    int usb_exp_vule_ing;
    int video_format_width;
    int video_format_height;
<<<<<<< HEAD
=======


    volatile int fd;
>>>>>>> 这一版增加了数字识别，但效果不太理想
};


