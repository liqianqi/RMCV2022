#include "Rmvision.h"
#include "DaHengCamera.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
using namespace Horizon;

<<<<<<< HEAD
namespace GxCamera{
int GX_exp_time = 1333;
=======
namespace GxCamera
{
int GX_exp_time = 2200;
>>>>>>> 这一版增加了数字识别，但效果不太理想
int GX_gain = 2;
DaHengCamera* camera_ptr_ = nullptr;
int GX_blance_r = 50;
int GX_blance_g = 32;
int GX_blance_b = 44;

<<<<<<< HEAD
=======
int GX_gamma = 1;

>>>>>>> 这一版增加了数字识别，但效果不太理想
//DaHengCamera* camera_ptr_ = nullptr;

void DaHengSetExpTime(int,void* ){
    camera_ptr_->SetExposureTime(GX_exp_time);
}

void DaHengSetGain(int,void* ){
    camera_ptr_->SetGain(3,GX_gain);
}

<<<<<<< HEAD
}

namespace UsbCamera {
=======
void DaHengSstGamma(int ,void* ){
    camera_ptr_->SetGamma(GX_gamma);
}

}

namespace UsbCamera
{
>>>>>>> 这一版增加了数字识别，但效果不太理想

myown::VideoCapture *usb_camera_ptr_ = nullptr;
int usb_exp_vule = 20;                           //usb相机曝光值
int usb_aperture;                           //usb相机亮度

void SetExpTime(int,void*){
    if(usb_camera_ptr_!=nullptr){
        usb_camera_ptr_->setExpousureTime(false,usb_exp_vule);
    }
}

void SetAperture(int,void*){
    if(usb_camera_ptr_!=nullptr){
        usb_camera_ptr_->setApertureNumber(usb_aperture);
    }
}

}

<<<<<<< HEAD
enum class DetectMode{
=======
enum class DetectMode
{
>>>>>>> 这一版增加了数字识别，但效果不太理想
    RUNE,
    ARMOR,
    NONE
};

<<<<<<< HEAD
void Rmvision::DaHengCameraProducer(){
=======
void Rmvision::DaHengCameraProducer()
{
    chrono::steady_clock::time_point ta = chrono::steady_clock::now();
>>>>>>> 这一版增加了数字识别，但效果不太理想
    while(true){

        if(GxCamera::camera_ptr_ != nullptr){
            while(image_buffer_rear_ - image_buffer_front_ > IMGAE_BUFFER){};
            if(GxCamera::camera_ptr_->GetMat(image_buffer_[image_buffer_rear_%IMGAE_BUFFER])){
                // 调整后，把这段注释掉

                ++image_buffer_rear_;
                // 收数开始
                serial_state_ = SerialState::RECEIVE;
<<<<<<< HEAD
=======

>>>>>>> 这一版增加了数字识别，但效果不太理想
            }
            else{
                delete GxCamera::camera_ptr_;
                GxCamera::camera_ptr_ = nullptr;
            }
<<<<<<< HEAD
=======
            datacontroler.getData(fd,stm32data);
            gimbal.pitch = stm32data.pitch_data_.f;
            gimbal.yaw = stm32data.yaw_data_.f;
>>>>>>> 这一版增加了数字识别，但效果不太理想
        }
        else{
            GxCamera::camera_ptr_ = new DaHengCamera;
            while(!GxCamera::camera_ptr_->StartDevice());
            GxCamera::camera_ptr_->SetResolution();
            while(!GxCamera::camera_ptr_->StreamOn());
            // 设置是否自动白平衡
            GxCamera::camera_ptr_->Set_BALANCE_AUTO(1);
            // 手动设置白平衡通道及系数，此之前需关闭自动白平衡

            GxCamera::camera_ptr_->SetExposureTime(GxCamera::GX_exp_time);
            GxCamera::camera_ptr_->SetGain(3, GxCamera::GX_gain);

            namedWindow("DaHengCameraDebug", CV_WINDOW_AUTOSIZE);
            createTrackbar("DaHengExpTime", "DaHengCameraDebug", &GxCamera::GX_exp_time, 10000,GxCamera::DaHengSetExpTime);
            GxCamera::DaHengSetExpTime(0,nullptr);
            createTrackbar("DaHengGain", "DaHengCameraDebug", &GxCamera::GX_gain, 10,GxCamera::DaHengSetGain);
            GxCamera::DaHengSetGain(0,nullptr);
<<<<<<< HEAD

            image_buffer_front_ = 0;
            image_buffer_rear_ = 0;
       }
=======
            createTrackbar("DaHengGamma", "DaHengCameraDebug", &GxCamera::GX_gamma, 100,GxCamera::DaHengSstGamma);
            //GxCamera::DaHengSetGain(0,nullptr);
            double GX_Gamma = 1.00 + double(GxCamera::GX_gamma/100);

            GXGetFloat(GxCamera::camera_ptr_->hDevice, GX_FLOAT_GAMMA_PARAM, &GX_Gamma);

            image_buffer_front_ = 0;
            image_buffer_rear_ = 0;
        }
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
        ta = t2;
        GET_IMAGE_FREQUENCE_ = 1/time_used.count();
>>>>>>> 这一版增加了数字识别，但效果不太理想
    }
}


<<<<<<< HEAD
void Rmvision::USBCameraProducer(){

//    cv::VideoCapture cap;
//    cap.open(0);
=======
void Rmvision::USBCameraProducer()
{

//    cv::VideoCapture cap;
//    cap.open(2);
>>>>>>> 这一版增加了数字识别，但效果不太理想
//    Mat src;

//    for(;;)
//    {
//        //cap.set(CV_CAP_PROP_EXPOSURE, 5);//曝光 -12
//        // 第一行没有必要加上
//        //while(image_buffer_rear_ - image_buffer_front_ > IMGAE_BUFFER);  //若满足这个条件，则让这个函数一只停在这里
//        cap >> image_buffer_[image_buffer_rear_%IMGAE_BUFFER];
//        src = image_buffer_[image_buffer_rear_%IMGAE_BUFFER].clone();

//        if(src.empty())
//            break;
//        image_buffer_rear_++;

//        // 收数开始
//        //serial_mutex_.lock();
////        serial_state_ = SerialState::RECEIVE;
////        gimbal.pitch = stm32data.pitch_data_.f;
////        gimbal.yaw = stm32data.yaw_data_.f;
//        //serial_mutex_.unlock();
//        double fps = cap.get(CV_CAP_PROP_FPS);
//        FPS = fps;
//        //cout << "                     " << fps << "              " << endl;
//    }

<<<<<<< HEAD
=======

    chrono::steady_clock::time_point ta = chrono::steady_clock::now();
>>>>>>> 这一版增加了数字识别，但效果不太理想
    while (1) {
        if(UsbCamera::usb_camera_ptr_ != nullptr){
            cout<<"进入制造图像"<<endl;
            while(image_buffer_rear_ - image_buffer_front_ > IMGAE_BUFFER-1);

            *UsbCamera::usb_camera_ptr_ >> image_buffer_[image_buffer_rear_%IMGAE_BUFFER];
            // 收数开始
            serial_state_ = SerialState::RECEIVE;
            ++image_buffer_rear_;
<<<<<<< HEAD
=======
            chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
            chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
            ta = t2;
            GET_IMAGE_FREQUENCE_ = 1/time_used.count();
            datacontroler.getData(fd,stm32data);
            gimbal.pitch = stm32data.pitch_data_.f;
            gimbal.yaw = stm32data.yaw_data_.f;
>>>>>>> 这一版增加了数字识别，但效果不太理想
        }else{

            cout<<"usb开始打开:"<<endl;
            UsbCamera::usb_camera_ptr_ = new myown::VideoCapture(USB_CAMERA_PATH ,3);
            UsbCamera::usb_camera_ptr_->setExpousureTime(false, usb_exp_vule);
            UsbCamera::usb_camera_ptr_->setApertureNumber(30);
            UsbCamera::usb_camera_ptr_->setVideoFormat(video_format_width, video_format_height, 1);
<<<<<<< HEAD
            UsbCamera::usb_camera_ptr_->info();
            UsbCamera::usb_camera_ptr_->startStream();

=======

            //UsbCamera::usb_camera_ptr_->info();
           //std::cout << "hello " << std::endl;
            UsbCamera::usb_camera_ptr_->startStream();


>>>>>>> 这一版增加了数字识别，但效果不太理想
            //创建滑动条
            namedWindow("UsbCameraDebug", CV_WINDOW_AUTOSIZE);
            createTrackbar("ExpTime", "UsbCameraDebug", &UsbCamera::usb_exp_vule, 200,UsbCamera::SetExpTime);

            UsbCamera::SetExpTime(0,nullptr);
<<<<<<< HEAD
            createTrackbar("ApertureValue", "UsbCameraDebug", &UsbCamera::usb_aperture, 128,UsbCamera::SetAperture);
=======
            createTrackbar("ApertureValue", "   UsbCameraDebug", &UsbCamera::usb_aperture, 128,UsbCamera::SetAperture);
>>>>>>> 这一版增加了数字识别，但效果不太理想

            image_buffer_front_ = 0;
            image_buffer_rear_ = 0;
            cout << "USB相机初始化完成" << endl;
        }
    }
}

<<<<<<< HEAD
void Rmvision::Fronted(){
=======
void Rmvision::Fronted()
{
>>>>>>> 这一版增加了数字识别，但效果不太理想

#ifdef CAMERAMODE
    USBCameraProducer();
#else
    DaHengCameraProducer();
#endif

}

<<<<<<< HEAD
void Rmvision::Backend(){
=======
void Rmvision::Backend()
{



    fd = OpenPort("/dev/ttyUSB0");

    configureSerial(fd);
>>>>>>> 这一版增加了数字识别，但效果不太理想

    Mat img;
    int i = 0;
    Mat srp = Mat::zeros(640,640,CV_8UC3);
    vector<Point2f> armor_2d_point;
<<<<<<< HEAD
    while(1){
        chrono::steady_clock::time_point ta = chrono::steady_clock::now();
=======
    chrono::steady_clock::time_point ta = chrono::steady_clock::now();
    while(1)
    {
>>>>>>> 这一版增加了数字识别，但效果不太理想
        while(image_buffer_rear_ <= image_buffer_front_);    //若满足这个条件，则让这个函数一只停在这里

        image_buffer_front_ = image_buffer_rear_ - 1;

        img = image_buffer_[image_buffer_front_%IMGAE_BUFFER].clone();
        // 上述步骤已经完成对图片的读取

        stm32data.flag = 0;

<<<<<<< HEAD
        if(stm32data.flag == 0){
            //imshow("",img);
            armor_number_ = armordector.StorePicture(img);

            if(armor_number_ == 0){
                is_armor_ = false;
                armor.center3d_  = Point3f(0,0,0);
            }else{
                armor.center3d_ = armordector.Getbest();
                is_armor_ = true;
            }
            //cout << "***********************************************************" <<  is_armor_ << endl;
            //armor.center3d_ = measuredistance.pnpResolving(armor_2d_point);

            //cam2ptz(gimbal,armor.center3d_);
=======
        if(stm32data.flag == 0)
        {
            //imshow("",img);
            armor_number_ = armordector.StorePicture(img);

            if(armor_number_ == 0)
            {
                is_armor_ = false;
                //armor.center3d_  = Point3f(0,0,0);
            }
            else
            {
                armor.center3d_ = armordector.Getbest();
                is_armor_ = true;
            }
            // cout << "***********************************************************" <<  is_armor_ << endl;
            // armor.center3d_ = measuredistance.pnpResolving(armor_2d_point);

            // cam2ptz(gimbal,armor.center3d_);
>>>>>>> 这一版增加了数字识别，但效果不太理想
            // 内部数据以毫米作单位
            predictor.getPrivateData(stm32data.init_firing_rate,armor,gimbal,is_armor_);
            gim = predictor.predictLocation();

            if(predictor.antiPamam())
            {

                //gim = predictor.predictLocation(); // 将预测的角度传出来
                T_f = predictor.returnAntiGyroData();

            }
//            else
//            {

//                gim = predictor.predictLocation(); // 将预测的角度传出来

//            }
<<<<<<< HEAD
            serial_mutex_.lock();
            /**/
            serial_state_ = SerialState::SEND;
=======
            //serial_mutex_.lock();
            /**/
            //serial_state_ = SerialState::SEND;
>>>>>>> 这一版增加了数字识别，但效果不太理想
            visiondata.yaw_data_.f = gim.yaw;
            visiondata.pitch_data_.f = gim.pitch;

//            visiondata.yaw_data_.f = -30.0;
//            visiondata.pitch_data_.f = -30.0;

            visiondata.time.f = (int)now();

            //visiondata.time.f = 1;

            datacontroler.state_ = 0;
            // is_armor_ = 1;
            visiondata.is_have_armor = is_armor_;
            // visiondata.is_have_armor = true;
            // 在修改完协议后，针对是否是陀螺，具体增加细节
            if(predictor.antiPamam())
            {
                start_fire = get<0>(T_f);
                t1 = now();
<<<<<<< HEAD
                while(start_fire < t1){
                    start_fire += get<1>(T_f);
                }
                if(abs(t1 - start_fire) <= 100 || (abs(t1 - t2) - get<1>(T_f)) < 100){
=======
                while(start_fire < t1)
                {
                    start_fire += get<1>(T_f);
                }
                if(abs(t1 - start_fire) <= 100 || (abs(t1 - t2) - get<1>(T_f)) < 100)
                {
>>>>>>> 这一版增加了数字识别，但效果不太理想
                    visiondata.is_fire = true;
                }
                t2 = now();// 记录开火时间
                visiondata.is_fire = false;
            }
<<<<<<< HEAD
            //visiondata.is_fire = true;
            serial_mutex_.unlock();
=======
            datacontroler.sentData(fd,visiondata);
            serial_state_ = SerialState::SEND;
            //visiondata.is_fire = true;
            //serial_mutex_.unlock();
>>>>>>> 这一版增加了数字识别，但效果不太理想
            if(display_mode == DisplayMode::Open)
            {
                char test[100];
                sprintf(test, "tz:%0.4f", armor.center3d_.z);
                cv::putText(img, test, cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, "tx:%0.4f", armor.center3d_.x);
                cv::putText(img, test, cv::Point(img.cols/3, 80), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, "ty:%0.4f", armor.center3d_.y);
                cv::putText(img, test, cv::Point(2*img.cols/3, 120), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, "get yaw:%0.4f ", stm32data.yaw_data_.f);
                cv::putText(img, test, cv::Point(10, 160), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, "get pitch:%0.4f ", stm32data.pitch_data_.f);
                cv::putText(img, test, cv::Point(img.cols/2, 160), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " yaw:%0.4f ", visiondata.yaw_data_.f);
                cv::putText(img, test, cv::Point(2, 200), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " pitch:%0.4f ", visiondata.pitch_data_.f);
                cv::putText(img, test, cv::Point(img.cols/2, 200), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " time:%d ", visiondata.time.f);
                cv::putText(img, test, cv::Point(10, 280), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                if(is_armor_){
                    sprintf(test, " is_find:%s ", "true");
                }else{
                    sprintf(test, " is_find:%s ", "false");
                }
                cv::putText(img, test, cv::Point(img.cols/3, 240), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                if(visiondata.is_fire){
                    sprintf(test, " is_fire:%s ", "true");
                }else{
                    sprintf(test, " is_fire:%s ", "false");
                }
                cv::putText(img, test, cv::Point(10, 240), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " time_stm32:%d ", stm32data.time.f);
                cv::putText(img, test, cv::Point(10, 360), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 6);

                if(stm32data.dubug_print){
                    sprintf(test, " is_get:%s ", "true");
                }else{
                    sprintf(test, " is_get:%s ", "false");
                }
                cv::putText(img, test, cv::Point(10, 400), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " v0:%0.4f ", stm32data.init_firing_rate);
                cv::putText(img, test, cv::Point(img.cols/2+50, 360), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

<<<<<<< HEAD
=======
                sprintf(test, " vx:%0.4f ", predictor.V_.x);
                cv::putText(img, test, cv::Point(10, 440), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " vy:%0.4f ", predictor.V_.y);
                cv::putText(img, test, cv::Point(10, 480), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " vz:%0.4f ", predictor.V_.z);
                cv::putText(img, test, cv::Point(10, 520), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " t1:%0.4f ", predictor.t1_);
                cv::putText(img, test, cv::Point(10, 560), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);
>>>>>>> 这一版增加了数字识别，但效果不太理想

            }
            char test[100];
            chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
            chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
<<<<<<< HEAD
            FPS = 1/time_used.count();

            sprintf(test, " FPS:%0.4f ", FPS);
            cv::putText(img, test, cv::Point(img.cols/2+50, 400), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

            if(display_mode == DisplayMode::Open){
=======
            ta = t2;
            FPS = 1/time_used.count();
            cout << "                                 "  << FPS << "                                 " << endl;
            sprintf(test, " FPS:%0.4f ", FPS);
            cv::putText(img, test, cv::Point(img.cols/2+50, 400), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

            sprintf(test, " GET_IMAGE_FREQUENCE_:%0.4f ", GET_IMAGE_FREQUENCE_);
            cv::putText(img, test, cv::Point(img.cols/2+50, 500), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

            if(display_mode == DisplayMode::Open)
            {
>>>>>>> 这一版增加了数字识别，但效果不太理想
                string windowName = "Data visualization";
                namedWindow(windowName, 0);
                imshow(windowName,img);
            }

<<<<<<< HEAD
            // serial_state_ = SerialState::WAIT;
        }else if (stm32data.flag == 1){
            cout << i++ << endl;
            
        }else{

        }
        chrono::steady_clock::time_point tb = chrono::steady_clock::now();
        chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(tb - ta);
        FPS = 1/time_used.count();
        cout << "                                 "  << FPS << "                                 " << endl;
=======
            //waitKey(2);

            // serial_state_ = SerialState::WAIT;
        }
        else if (stm32data.flag == 1)
        {
            cout << i++ << endl;
            
        }
        else
        {

        }


>>>>>>> 这一版增加了数字识别，但效果不太理想

    }

}

void Rmvision::Sent_Receive(){

    int fd;

<<<<<<< HEAD
    fd = OpenPort("/dev/ttyUSB0");
=======
    fd = OpenPort("/dev/ttyUSB1");
>>>>>>> 这一版增加了数字识别，但效果不太理想

    configureSerial(fd);
    while(true){

        //fd = OpenPort("/dev/ttyUSB1");
        // 收数
        serial_mutex_.lock();
        //serial_state_ = SerialState::RECEIVE;
        datacontroler.getData(fd,stm32data);
        gimbal.pitch = stm32data.pitch_data_.f;
        gimbal.yaw = stm32data.yaw_data_.f;
        //serial_state_ = SerialState::WAIT;            //停止发数
        serial_mutex_.unlock();
        // 发数
        //serial_state_ = SerialState::SEND;
<<<<<<< HEAD
        if(serial_state_ == SerialState::SEND){
            //自瞄和能量机关
            serial_mutex_.lock();
            datacontroler.sentData(fd,visiondata);
            serial_state_ = SerialState::WAIT;            //停止发数
            //waitKey(10);
            serial_mutex_.unlock();

        }
=======

            //自瞄和能量机关
            serial_mutex_.lock();
            datacontroler.sentData(fd,visiondata);
            //serial_state_ = SerialState::WAIT;            //停止发数
            //waitKey(10);
            serial_mutex_.unlock();


>>>>>>> 这一版增加了数字识别，但效果不太理想
    }
}
