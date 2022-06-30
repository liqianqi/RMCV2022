#include "Rmvision.h"
#include "DaHengCamera.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
using namespace Horizon;


namespace GxCamera
{
int GX_exp_time = 2533;

int GX_gain = 4;
DaHengCamera* camera_ptr_ = nullptr;
int GX_blance_r = 50;
int GX_blance_g = 32;
int GX_blance_b = 44;


int GX_gamma = 1;

//DaHengCamera* camera_ptr_ = nullptr;

void DaHengSetExpTime(int,void* ){
    camera_ptr_->SetExposureTime(GX_exp_time);
}

void DaHengSetGain(int,void* ){
    camera_ptr_->SetGain(3,GX_gain);
}

}

namespace UsbCamera
{

myown::VideoCapture *usb_camera_ptr_ = nullptr;
int usb_exp_vule = 48;                           // usb相机曝光值
int usb_aperture = 22;                           // usb相机亮度

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


enum class DetectMode
{
    RUNE,
    ARMOR,
    NONE
};


void Rmvision::DaHengCameraProducer()
{
    chrono::steady_clock::time_point ta = chrono::steady_clock::now();
    while(true)
    {
        if(GxCamera::camera_ptr_ != nullptr){
            while(image_buffer_rear_ - image_buffer_front_ > IMGAE_BUFFER){};
            if(GxCamera::camera_ptr_->GetMat(image_buffer_[image_buffer_rear_%IMGAE_BUFFER])){
                // 调整后，把这段注释掉

                ++image_buffer_rear_;
                // 收数开始
                serial_state_ = SerialState::RECEIVE;


            }
            else{
                delete GxCamera::camera_ptr_;
                GxCamera::camera_ptr_ = nullptr;
            }

        }
        else
        {
            GxCamera::camera_ptr_ = new DaHengCamera;
            while(!GxCamera::camera_ptr_->StartDevice());
            GxCamera::camera_ptr_->SetResolution();
            while(!GxCamera::camera_ptr_->StreamOn());
            // 设置是否自动白平衡
            GxCamera::camera_ptr_->Set_BALANCE_AUTO(1);
            // 手动设置白平衡通道及系数，此之前需关闭自动白平衡

            GxCamera::camera_ptr_->SetExposureTime(GxCamera::GX_exp_time);
            GxCamera::camera_ptr_->SetGain(3, GxCamera::GX_gain);

            double GX_Gamma = 2.85;
            GxCamera::camera_ptr_->setGamma(GX_Gamma);

            namedWindow("DaHengCameraDebug", WINDOW_AUTOSIZE);
            createTrackbar("DaHengExpTime", "DaHengCameraDebug", &GxCamera::GX_exp_time, 10000,GxCamera::DaHengSetExpTime);
            GxCamera::DaHengSetExpTime(0,nullptr);
            createTrackbar("DaHengGain", "DaHengCameraDebug", &GxCamera::GX_gain, 10,GxCamera::DaHengSetGain);
            GxCamera::DaHengSetGain(0,nullptr);
           //GxCamera::DaHengSetGain(0,nullptr);

            image_buffer_front_ = 0;
            image_buffer_rear_ = 0;
        }
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
        ta = t2;
        GET_IMAGE_FREQUENCE_ = 1/time_used.count();

        serial_mutex_.lock();

        datacontroler.getData(fd,stm32data);
        gimbal.pitch = stm32data.pitch_data_.f;
        gimbal.yaw = stm32data.yaw_data_.f;

        serial_mutex_.unlock();

    }
}

void Rmvision::USBCameraProducer()
{

//    cv::VideoCapture cap;
//    cap.open(2);
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

    chrono::steady_clock::time_point ta = chrono::steady_clock::now();
    while (1) {
        if(UsbCamera::usb_camera_ptr_ != nullptr){
            cout<<"进入制造图像"<<endl;
            while(image_buffer_rear_ - image_buffer_front_ > IMGAE_BUFFER-1);

            *UsbCamera::usb_camera_ptr_ >> image_buffer_[image_buffer_rear_%IMGAE_BUFFER];
            // 收数开始
            serial_state_ = SerialState::RECEIVE;
            ++image_buffer_rear_;

            chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
            chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
            ta = t2;
            GET_IMAGE_FREQUENCE_ = 1/time_used.count();
            serial_mutex_.lock();
            datacontroler.getData(fd,stm32data);
            gimbal.pitch = stm32data.pitch_data_.f;
            gimbal.yaw = stm32data.yaw_data_.f;

            serial_mutex_.unlock();
        }else{

            cout<<"usb开始打开:"<<endl;
            UsbCamera::usb_camera_ptr_ = new myown::VideoCapture(USB_CAMERA_PATH ,3);
            UsbCamera::usb_camera_ptr_->setExpousureTime(false, usb_exp_vule);
            UsbCamera::usb_camera_ptr_->setApertureNumber(30);
            UsbCamera::usb_camera_ptr_->setVideoFormat(video_format_width, video_format_height, 1);
            //UsbCamera::usb_camera_ptr_->info();
            UsbCamera::usb_camera_ptr_->startStream();

            //UsbCamera::usb_camera_ptr_->info();
           //std::cout << "hello " << std::endl;
            // UsbCamera::usb_camera_ptr_->startStream();

            //创建滑动条
            namedWindow("UsbCameraDebug", WINDOW_AUTOSIZE);

            createTrackbar("ExpTime", "UsbCameraDebug", &UsbCamera::usb_exp_vule, 200,UsbCamera::SetExpTime);
            UsbCamera::SetExpTime(0,nullptr);

            createTrackbar("ApertureValue", "UsbCameraDebug", &UsbCamera::usb_aperture, 128,UsbCamera::SetAperture);
            UsbCamera::SetAperture(0,nullptr);
            image_buffer_front_ = 0;
            image_buffer_rear_ = 0;
            cout << "USB相机初始化完成" << endl;
        }
    }
}


void Rmvision::Fronted()
{

#ifdef CAMERAMODE
    USBCameraProducer();
#else
    DaHengCameraProducer();
#endif

}

void Rmvision::Backend()
{
    fd = OpenPort("/dev/ttyUSB0");

    configureSerial(fd);

    Mat img;
    int i = 0;
    Mat srp = Mat::zeros(640,640,CV_8UC3);
    vector<Point2f> armor_2d_point;
    chrono::steady_clock::time_point ta = chrono::steady_clock::now();
    chrono::steady_clock::time_point time_of_start = chrono::steady_clock::now();
    while(1)
    {
        while(image_buffer_rear_ <= image_buffer_front_);    // 若满足这个条件，则让这个函数一只停在这里

        chrono::steady_clock::time_point time_of_process = chrono::steady_clock::now();
        chrono::duration<double> time_run = chrono::duration_cast <chrono::duration < double>>(time_of_process - time_of_start);
        image_buffer_front_ = image_buffer_rear_ - 1;

        img = image_buffer_[image_buffer_front_%IMGAE_BUFFER].clone();
        // 上述步骤已经完成对图片的读取

        stm32data.flag = 0;

        if(stm32data.flag == 0)
        {
            //大概是5-6ms
            //cout << "hello " << endl;
            armor_number_ = armordector.StorePicture(img);
            //cout << "hello " << endl;

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
            is_arnor_last_ = is_armor_;
            // cout << "***********************************************************" <<  is_armor_ << endl;
            // armor.center3d_ = measuredistance.pnpResolving(armor_2d_point);

            // cam2ptz(gimbal,armor.center3d_);

            // 内部数据以毫米作单位
            //stm32data.init_firing_rate = 16;
            chrono::steady_clock::time_point t1_ = chrono::steady_clock::now();

            predictor.getPrivateData(stm32data.init_firing_rate,armor,gimbal,is_armor_,(1/FPS));
            gim = predictor.predictLocation();

            if(predictor.antiPamam())
            {
                T_f = predictor.returnAntiGyroData();
            }

            //chrono::steady_clock::time_point t1_ = chrono::steady_clock::now();

            chrono::steady_clock::time_point tc = chrono::steady_clock::now();
            chrono::duration<double> time_used1 = chrono::duration_cast <chrono::duration < double>>(tc - t1_);
            
            cout << "time_used is  :   "  << time_used1.count()*1000 << "            " << endl;

            serial_mutex_.lock();

            visiondata.yaw_data_.f = gim.yaw;
            visiondata.pitch_data_.f = gim.pitch;

            chrono::steady_clock::time_point time_of_now_ = chrono::steady_clock::now();
            chrono::duration<double> time_now = chrono::duration_cast <chrono::duration < double>>(time_of_now_ - time_of_start);

            if(int(time_now.count()*1000)%1000 == 0)
            {
                visiondata.time.f = time_now.count()*1000;
                datacontroler.is_start_time_stamp_ = true;
            }
            else
            {
                visiondata.time.f = time_run.count()*1000;
                datacontroler.is_start_time_stamp_ = false;
            }

            //visiondata.time.f = 1;

            datacontroler.state_ = 0;
            // is_armor_ = 1;
            visiondata.is_have_armor = is_armor_;

            visiondata.pitch_w_rads.f = armordector.height_of_led_;
            visiondata.yaw_w_rads.f = predictor.V_.y;
            // visiondata.is_have_armor = true;
            // 在修改完协议后，针对是否是陀螺，具体增加细节
            if(predictor.antiPamam())
            {
                start_fire = get<0>(T_f);
                t1 = now();

                while(start_fire < t1)
                {
                    start_fire += get<1>(T_f);
                }
                if(abs(t1 - start_fire) <= 100 || (abs(t1 - t2) - get<1>(T_f)) < 100)
                {

                    visiondata.is_fire = true;
                }
                t2 = now();// 记录开火时间
                visiondata.is_fire = false;
            }


            datacontroler.sentData(fd,visiondata);


//             float distance = std::sqrt(armor.center3d_.x*armor.center3d_.x + armor.center3d_.y*armor.center3d_.y + armor.center3d_.z);

            serial_mutex_.unlock();
            //predictor.visual_point3D_.z
            if(display_mode == DisplayMode::Open)
            {
                char test[100];
                sprintf(test, "tz:%0.4f",predictor.visual_point3D_.z);
                cv::putText(img, test, cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, "tx:%0.4f", predictor.visual_point3D_.x);
                cv::putText(img, test, cv::Point(img.cols/3, 80), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, "ty:%0.4f", predictor.visual_point3D_.y);
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

                if(stm32data.dubug_print)
                {
                    sprintf(test, " is_get:%s ", "true");
                }
                else
                {
                    sprintf(test, " is_get:%s ", "false");
                }
                cv::putText(img, test, cv::Point(10, 400), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " v0:%0.4f ", stm32data.init_firing_rate);
                cv::putText(img, test, cv::Point(img.cols/2+50, 360), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " vx:%0.4f ", predictor.V_.x);
                cv::putText(img, test, cv::Point(10, 440), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " vy:%0.4f ", predictor.V_.y);
                cv::putText(img, test, cv::Point(10, 480), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " vz:%0.4f ", predictor.V_.z);
                cv::putText(img, test, cv::Point(10, 520), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                sprintf(test, " t1:%0.4f ", predictor.t1_);
                cv::putText(img, test, cv::Point(10, 560), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

            }

            char test[100];
            chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
            chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);

            ta = t2;
            FPS = 1/time_used.count();
            cout << "                                 "  << FPS << "                                 " << endl;
            sprintf(test, " FPS:%0.4f ", FPS);
            cv::putText(img, test, cv::Point(img.cols/2+50, 400), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

            sprintf(test, " GET_IMAGE_FREQUENCE_:%0.4f ", GET_IMAGE_FREQUENCE_);
            cv::putText(img, test, cv::Point(img.cols/2+50, 500), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

            if(display_mode == DisplayMode::Open)
            {
                string windowName = "Data visualization";
                namedWindow(windowName, 0);
                imshow(windowName,img);
            }

            //waitKey(500);

            // serial_state_ = SerialState::WAIT;
        }
        else if (stm32data.flag == 1)
        {
            cout << i++ << endl;
            
        }
        else
        {

        }

    }

}

void Rmvision::Sent_Receive(){

}
