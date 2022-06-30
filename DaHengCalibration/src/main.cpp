#include <DaHengCamera.h>
#include <DxImageProc.h>
#include <GxIAPI.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

DaHengCamera* camera_ptr_ = nullptr;
namespace GxCamera{
int GX_exp_time = 10000;
int GX_gain = 10;

int GX_blance_r = 50;
int GX_blance_g = 32;
int GX_blance_b = 44;

DaHengCamera* camera_ptr_ = nullptr;

void DaHengSetExpTime(int,void* ){
    camera_ptr_->SetExposureTime(GX_exp_time);
}

void DaHengSetGain(int,void* ){
    camera_ptr_->SetGain(3,GX_gain);
}
}
int main(){
    Mat img;
    int i = 1;
    while(true){
        if(GxCamera::camera_ptr_ != nullptr){
            //while(image_buffer_rear_ - image_buffer_front_ > IMGAE_BUFFER-1);
            if(GxCamera::camera_ptr_->GetMat(img)){
                int key = waitKey(2);
                Mat &src = img;
                if(key == ' '){
                    if(key == ' '){
                        string windowName = "show";
                        namedWindow(windowName, 0);
                        cv::imwrite("/home/liqianqi/桌面/images/image"+to_string(i++)+".jpg", src);
                        imshow(windowName,src);
                    }

                }
                string windowName = "Data visualization";
                namedWindow(windowName, 0);
                imshow(windowName,img);
                //GxCamera::camera_ptr_->SetExposureTime(GxCamera::GX_exp_time);
                //GxCamera::camera_ptr_->SetGain(3, GxCamera::GX_gain);
            }
            else{
                delete GxCamera::camera_ptr_;
                GxCamera::camera_ptr_ = nullptr;
            }
        }
        else{
            GxCamera::camera_ptr_ = new DaHengCamera;
            while(!GxCamera::camera_ptr_->StartDevice());
            GxCamera::camera_ptr_->SetResolution();
            while(!GxCamera::camera_ptr_->StreamOn());
            //设置是否自动白平衡
            GxCamera::camera_ptr_->Set_BALANCE_AUTO(0);
            //手动设置白平衡通道及系数，此之前需关闭自动白平衡

            GxCamera::camera_ptr_->SetExposureTime(GxCamera::GX_exp_time);
            GxCamera::camera_ptr_->SetGain(3, GxCamera::GX_gain);

            namedWindow("DaHengCameraDebug", CV_WINDOW_AUTOSIZE);
            createTrackbar("DaHengExpTime", "DaHengCameraDebug", &GxCamera::GX_exp_time, 10000,GxCamera::DaHengSetExpTime);
            GxCamera::DaHengSetExpTime(0,nullptr);
            createTrackbar("DaHengGain", "DaHengCameraDebug", &GxCamera::GX_gain, 10,GxCamera::DaHengSetGain);
            GxCamera::DaHengSetGain(0,nullptr);


        }

    }
}





