#include "ArmorDector.h"
#include<vector>

int threshold_param = 158;
<<<<<<< HEAD
int element_size = 2;
=======
int element_size = 1;
int element_size1 = 1;
>>>>>>> 这一版增加了数字识别，但效果不太理想
//#define display_mode DisplayMode::Open
//#define display_mode DisplayMode::Close

#define color_ Color::Blue
//#define color_  Color::Red
//int i = 0;
//int max1 = 255;
unsigned short ArmorDector::StorePicture(Mat &img){
    //imshow("hhh",img);
    src_image_ = img;
    binary_image_ = img;
<<<<<<< HEAD
=======

>>>>>>> 这一版增加了数字识别，但效果不太理想
    if(display_mode) show_image_ = img.clone();
    if(display_mode) aultimate_image_ = img;
    SeparateColor();
    led_num = GetAllLed();
    armor_num_ = GetAllArmor(led_num);
    AngleResolving(armor_num_);
    //Select_Best(armor_num_);
<<<<<<< HEAD
    Getbest();
    //cout << "***********************************************************" <<  armor_num_ << endl;
=======
    //Getbest();
    //cout << "***********************************************************" <<  armor_num_ << endl;
    if(armor_num_)
    {
        last_is_have_target_ = true;
    }
    else
    {
        last_is_have_target_ = false;
    }
>>>>>>> 这一版增加了数字识别，但效果不太理想
    return armor_num_;
}


void ArmorDector::SeparateColor(){

    Mat splited_image[3];

    //cvtColor(src_image_,binary_image_, CV_BGR2GRAY);
    //threshold(binary_image_,binary_image_, 180, 255, THRESH_BINARY);

    split(src_image_, splited_image);
    if(color_ == Color::Blue)
        channel_image_ = splited_image[0] - 0.2f*splited_image[1] - 0.8f*splited_image[2];//蓝色
    else channel_image_ = splited_image[2] - 0.2f*splited_image[1] - 0.8f*splited_image[0];//红色

    //namedWindow("yanse", CV_WINDOW_AUTOSIZE);
//    createTrackbar("i", "yanse", &i,max1);
    threshold(channel_image_, channel_image_,threshold_param, 255, THRESH_BINARY);

    namedWindow("threshold", CV_WINDOW_AUTOSIZE);
    createTrackbar("threshold", "threshold", &threshold_param,255);

<<<<<<< HEAD
    Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(element_size, element_size));
    //Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    //erode(channel_image_,channel_image_,element);
    dilate(channel_image_, channel_image_, element);

    final_image_ = channel_image_;

    //Canny(final_image_,final_image_,50,250);//Canny检测边缘
    morphologyEx(final_image_,final_image_, MORPH_CLOSE, element);//防止小洞，使用
    namedWindow("threshold", CV_WINDOW_AUTOSIZE);
    createTrackbar("element_size", "threshold", &element_size,12);

    //imshow("channel_image_",channel_image_);
    //imshow("binary_image_",binary_image_);
    //imshow("final_image_",final_image_);
=======
//    if(last_is_have_target_)
//    {
//        //final_image_ = channel_image_(Rect(last_armor_.center2d_.x - 300, last_armor_.center2d_.y - 300 , 600,600));
//    }
    Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(element_size, element_size));
    Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(element_size1, element_size1));

    erode(channel_image_,channel_image_,element);

    dilate(channel_image_, channel_image_, element1);
    
    final_image_ = channel_image_;

    //Canny(final_image_,final_image_,50,250);//Canny检测边缘
    //morphologyEx(final_image_,final_image_, MORPH_OPEN, element);//防止小洞，使用
    //dilate(final_image_, final_image_, 6);
    //namedWindow("threshold", CV_WINDOW_AUTOSIZE);
    createTrackbar("element_size", "threshold", &element_size,12);
    createTrackbar("element_size1", "threshold", &element_size1,12);
    
>>>>>>> 这一版增加了数字识别，但效果不太理想

    if(display_mode == DisplayMode::Open)
    {
        string windowName = "处理后的图片";
        namedWindow(windowName, 0);
        imshow(windowName,final_image_);
    }
<<<<<<< HEAD
//    split(src_image_, splited_image);
//    if(color_)
//        binary_image_ = splited_image[0] - 0.2f*splited_image[1] - 0.8f*splited_image[2];//蓝色
//    else binary_image_ = splited_image[2] - 0.2f*splited_image[1] - 0.8f*splited_image[0];//红色

//    threshold(binary_image_,binary_image_,85,255,THRESH_BINARY);

//    Canny(binary_image_,binary_image_,50,250);//Canny检测边缘

//    threshold(binary_image_,binary_image_,70,255,CV_THRESH_BINARY);//设定轮廓阈值

//    if(display_mode) imshow("二值化图片",binary_image_);

//    GetAllLed();

=======
>>>>>>> 这一版增加了数字识别，但效果不太理想

}



unsigned short ArmorDector::GetAllLed(){
    vector<std::vector<cv::Point>> led_contours;
    unsigned short led_num= 0;
    vector<cv::Vec4i> hierarchy;
    RotatedRect led_rect;

    findContours(final_image_, led_contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);// RETR_EXTERNAL
    //findContours(binary_image_, led_contours, hierarchy,RETR_EXTERNAL, CHAIN_APPROX_NONE);// RETR_EXTERNAL

    unsigned short contours_size = static_cast<unsigned short>(led_contours.size());

    for(unsigned short contour_index = 0; contour_index < contours_size && led_num < LED_SIZE; contour_index++){

        vector<cv::Point> & led_contour = led_contours[contour_index];
        double area = contourArea(led_contour);

<<<<<<< HEAD
        if(area < 15 || area > 900) continue;//5 100
=======
        if(area < 20 || area > 2000) continue;//5 100
>>>>>>> 这一版增加了数字识别，但效果不太理想
        //cout<<"面积为"<<area<<endl;
        led_rect = minAreaRect(led_contour);

        Led led;
        Point2f vertex[4];

        led_rect.points(vertex);
        //尝试
        sort(vertex, vertex + 4, ledCmpY);
        sort(vertex, vertex + 2, ledCmpX);
        sort(vertex+2, vertex+4, ledCmpX);
        const Point2f& bottomLeft = vertex[2];
        const Point2f& topLeft = vertex[0];
        const Point2f& topRight = vertex[1];
        const Point2f& bottomRight = vertex[3];

        led.top_point_ = (topLeft + topRight) * 0.5f;
        led.bottom_point_ = (bottomLeft + bottomRight) * 0.5f;
        led.center_ =  (led.top_point_ + led.bottom_point_) * 0.5f;

        Point2f point_delta_vertical = led.bottom_point_ - led.top_point_;
        led.length_ = sqrt(point_delta_vertical.x * point_delta_vertical.x + point_delta_vertical.y * point_delta_vertical.y);
        led.slope_ = point_delta_vertical.x / point_delta_vertical.y;

        Point2f point_delta_horizontal = (topRight - topLeft);
        led.width_ = sqrt(point_delta_horizontal.x * point_delta_horizontal.x + point_delta_horizontal.y * point_delta_horizontal.y);
        led.ratio_ = led.length_ / led.width_;

        if( led.ratio_ > contants::kLedMaxHeightVSWidth
            || led.ratio_ < contants::kLedMinHeightVSWidth
            || abs(led.slope_) > contants::kLedMaxSlope
        ) continue;

        led_array_[led_num++] = led;
        if(display_mode == DisplayMode::Open)
        {
            line(show_image_, topLeft, topRight,Scalar(255, 0, 255), 2, 8);
            line(show_image_, topLeft, bottomLeft,Scalar(255, 0, 255), 2, 8);
            line(show_image_, bottomRight, topRight,Scalar(255, 0, 255), 2, 8);
            line(show_image_, bottomRight, bottomLeft,Scalar(255, 0, 255), 2, 8);
        }

    }

    if(display_mode == DisplayMode::Open)
    {
        string windowName = "led";
        namedWindow(windowName, 0);
        imshow(windowName, show_image_);
    }
    //if(display_mode)cout<<"灯条数目为"<<led_num<<endl;
    waitKey(1);

    return led_num;
}

unsigned short ArmorDector::GetAllArmor(int led_num){
    unsigned short armor_num = 0;
    bool is_selected[led_num];

    memset(is_selected, false, led_num);
    sort(led_array_, led_array_ + led_num, ledCmpLength);

    for(unsigned short i = 0; i < led_num; i++){
        for(unsigned short index = i+1; index < led_num; index++){
<<<<<<< HEAD
            if(is_selected[i] || is_selected[index]) continue; //未被配对

            float lRatio = led_array_[i].length_ / led_array_[index].length_; //长度相近
            if(lRatio > 1.5f || lRatio < 0.66) continue;

            float AngleMinus = led_array_[i].slope_ - led_array_[index].slope_; //角度相近
            if(abs(AngleMinus) > 10.0f) continue;

            cv::Point2f center_minus = led_array_[i].center_ - led_array_[index].center_;

            float centerDeltaY = abs(center_minus.y);
            float centerDeltaX = abs(center_minus.x);
            float centerDistance = sqrt(center_minus.y * center_minus.y + center_minus.x * center_minus.x);
            float length = (led_array_[i].length_+led_array_[index].length_) * 0.5f;
            float hRatio =  centerDeltaY / length;
            float wRatio =  centerDeltaX / length;
            float whRatio = centerDistance / length;

            if( hRatio > 0.9f || wRatio > 6.0f || whRatio > 16.0f || whRatio < 0.9f) continue;
=======
            //if(is_selected[i] || is_selected[index]) continue; //未被配对

            float lRatio = led_array_[i].length_ / led_array_[index].length_; //长度相近

            if(lRatio > 1.30f || lRatio < 0.70f) 
            {
                cout << "长度不符合" << endl;
                continue;
            }
            //            cout << led_array_[i].slope_ << endl;
            //            cout << led_array_[index].slope_ << endl;
                        //cout << led_array_[i].slope_  - led_array_[index].slope_  << endl;

            
            // 两个灯条的夹角差，主要去掉内外八
            float AngleMinus = std::atan(led_array_[i].slope_)*180/CV_PI - std::atan(led_array_[index].slope_)*180/CV_PI; //角度相近
            //            cout << std::abs(AngleMinus) << endl;
            //            cout << endl << endl << endl;

            if(std::abs(AngleMinus) > 9.6f) 
            {
                cout << "角度不符合" << endl;
                continue;
            }

>>>>>>> 这一版增加了数字识别，但效果不太理想

            is_selected[i] = true;
            is_selected[index] = true;
            Armor armor;
            armor.left_led_[0] = led_array_[i].top_point_;
            armor.left_led_[1] = led_array_[i].bottom_point_;
            armor.right_led_[0] = led_array_[index].top_point_;
            armor.right_led_[1] = led_array_[index].bottom_point_;

<<<<<<< HEAD
            if(led_array_[i].center_.x > led_array_[index].center_.x){
                swap(armor.left_led_, armor.right_led_);
            }

            //armor.width = centerDistance;
            if(whRatio < contants::kArmorTypeThreshold){
                armor.armor_type_ = Armor::ArmorType::SMALL;
            }
            else{
                armor.armor_type_ = Armor::ArmorType::LARGE;
            }
            armor.center2d_.x = ( armor.left_led_[0].x +  armor.right_led_[1].x)/2;
            armor.center2d_.y = ( armor.left_led_[0].y +  armor.right_led_[1].y)/2;
            armor_array_[armor_num] = armor;
            cout << armor_array_[armor_num].center2d_.x << "    " << armor_array_[armor_num].center2d_.y << endl;
=======

            float width_height_ratio = std::sqrt((std::pow((armor.left_led_[0].x - armor.left_led_[1].x),2)) + pow((armor.left_led_[0].y - armor.left_led_[1].y),2)) 
                                        / std::sqrt(std::pow(armor.left_led_[0].x-armor.right_led_[0].x ,2) + std::pow(armor.left_led_[0].y - armor.right_led_[0].y ,2));

            cout << width_height_ratio << endl;
            
            if(!((width_height_ratio > 0.20 && width_height_ratio < 0.28) || (width_height_ratio > 0.28 && width_height_ratio < 0.35)))
            {
                
                cout << "长宽比例不符合" << endl;
                continue;
            }

            float a = std::sqrt((std::pow((armor.left_led_[0].x - armor.left_led_[1].x),2)) + pow((armor.left_led_[0].y - armor.left_led_[1].y),2));
            float b = std::sqrt((std::pow((armor.right_led_[0].x - armor.left_led_[0].x),2)) + std::pow((armor.right_led_[0].y - armor.left_led_[0].y),2));
            float c = std::sqrt((std::pow(armor.right_led_[0].x+armor.left_led_[1].x,2))+std::pow(armor.right_led_[0].y+armor.left_led_[1].y,2));

            float angleHeightWidthRatio = std::acos((a*a + b*b -c*c)/2*a*b);

            if(angleHeightWidthRatio > 120 || angleHeightWidthRatio < 80)
            {
                cout << "装甲板矩形不符合标准" << endl;
                continue;
            }



            if(led_array_[i].center_.x > led_array_[index].center_.x)
            {
                swap(armor.left_led_, armor.right_led_);
            }

            /*
                float delta_y_2d = std::abs(armor.right_led_[0].y - armor.right_led_[1].y);
                float delta_x_2d = std::abs(armor.right_led_[0].x - armor.right_led_[1].x);

                Point2f iso_point = Point2f(armor.left_led_[0].x + delta_x_2d , armor.left_led_[0].y + delta_y_2d);

                float a = led_array_[i].length_;
                float b = led_array_[index].length_;
                float c = std::sqrt(pow((armor.left_led_[1].x - iso_point.x),2) + pow((armor.left_led_[1].y - iso_point.y),2));

                armor.angle_ = std::acos((a*a + b*b - c*c)/2*a*b);

                if(armor.angle_ > 1)
                {
                    continue;
                }
            */



            /// 数字识别
            // *******************************
            //armor.id = classifier(aultimate_image_);

            // if(!(armor.id && armor.id != -1))
            // {
            //     continue;
            // }
            
            //cout << armor.id << endl;
            // *******************************



            //armor.width = centerDistance;
            
            if(width_height_ratio > contants::kArmorTypeThreshold)
            {
                cout << "small" << endl;
                armor.armor_type_ = Armor::ArmorType::SMALL;
            }
            else
            {
                cout << "big" << endl;
                armor.armor_type_ = Armor::ArmorType::LARGE;
            }

            //armor.armor_type_ = Armor::ArmorType::LARGE;
            
            armor.center2d_.x = ( armor.left_led_[0].x +  armor.right_led_[1].x)/2;
            armor.center2d_.y = ( armor.left_led_[0].y +  armor.right_led_[1].y)/2;
            armor_array_[armor_num] = armor;
            //cout << armor_array_[armor_num].center2d_.x << "    " << armor_array_[armor_num].center2d_.y << endl;
>>>>>>> 这一版增加了数字识别，但效果不太理想

            if(display_mode == DisplayMode::Open)
            {
                line(aultimate_image_, armor.left_led_[0], armor.left_led_[1],Scalar(0, 0, 255), 1, 8);
                line(aultimate_image_, armor.right_led_[0], armor.left_led_[0],Scalar(0, 0, 255), 1, 8);
                line(aultimate_image_, armor.right_led_[1], armor.left_led_[1],Scalar(0, 0, 255), 1, 8);
                line(aultimate_image_, armor.right_led_[1], armor.right_led_[0],Scalar(0, 0, 255), 1, 8);
<<<<<<< HEAD
                if(armor.armor_type_ == Armor::ArmorType::SMALL){
                    circle(aultimate_image_,Point(armor_array_[armor_num].center2d_.x,armor_array_[armor_num].center2d_.y),3,Scalar(255, 255, 255));
                }else{
=======
                if(armor.armor_type_ == Armor::ArmorType::SMALL)
                {
                    circle(aultimate_image_,Point(armor_array_[armor_num].center2d_.x,armor_array_[armor_num].center2d_.y),3,Scalar(255, 255, 255));
                }
                else
                {
>>>>>>> 这一版增加了数字识别，但效果不太理想
                    circle(aultimate_image_,Point(armor_array_[armor_num].center2d_.x,armor_array_[armor_num].center2d_.y),3,Scalar(255, 0, 255));
                }
            }
            armor_num++;
        }
        //if(armor_num == ARMOR_SIZE) break;
    }

    return armor_num;

}



void ArmorDector::AngleResolving(const unsigned short& armor_num){

    vector<cv::Point2f> image_coodinate_points;
    vector<cv::Point3f> object_coodinate_points;

    for(unsigned short index = 0; index < armor_num; index++){
        GetImagePointData(armor_array_[index], image_coodinate_points);
        GetObjectPointData(armor_array_[index], object_coodinate_points);
        SolveAngle(armor_array_[index], image_coodinate_points, object_coodinate_points);
    }

    if(armor_num == 1){
        best_armor = armor_array_[0];
        //cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << best_armor.center3d_.x << endl;
        best_armor.is_findTarget = 1;
<<<<<<< HEAD
    }else if(armor_num == 0){
        best_armor.center3d_.x = 0;
        best_armor.center3d_.y = 0;
        best_armor.center3d_.z = 0;
=======
        last_armor_ = best_armor;
    }else if(armor_num == 0){
         best_armor = last_armor_;
>>>>>>> 这一版增加了数字识别，但效果不太理想
    }
    else
        Select_Best(armor_num);
    
}


void ArmorDector::GetImagePointData(const Armor& armor, std::vector<cv::Point2f>& point2D){

    point2D.push_back(armor.left_led_[0]);
    point2D.push_back(armor.right_led_[0]);
    point2D.push_back(armor.right_led_[1]);
    point2D.push_back(armor.left_led_[1]);

}


void ArmorDector::GetObjectPointData(const Armor& armor, std::vector<cv::Point3f>& point3D){
    float fHalfX;
    float fHalfY;

    switch(armor.armor_type_){
        case Armor::ArmorType::SMALL :{
            fHalfX = contants::kRealSmallArmorWidth * 0.5f;     //将装甲板的宽的一半作为原点的x
            fHalfY = contants::kRealSmallArmorHeight * 0.5f;    //将装甲板的宽的一半作为原点的y
            break;
        }
        case Armor::ArmorType::LARGE:{
            fHalfX = contants::kRealLargeArmorWidth * 0.5f;
            fHalfY = contants::kRealLargeArmorHeight * 0.5f;
            break;
        }
        case Armor::ArmorType::RUNE:{
            fHalfX = contants::kRealRuneWidth * 0.5f;
            fHalfY = contants::kRealRuneHeight * 0.5f;
            break;
        }
        default:
            break;
    }

    point3D.push_back(cv::Point3f(-fHalfX,fHalfY, 0.0f));
    point3D.push_back(cv::Point3f(fHalfX,fHalfY, 0.0f));
    point3D.push_back(cv::Point3f(fHalfX,-fHalfY, 0.0f));
    point3D.push_back(cv::Point3f(-fHalfX,-fHalfY, 0.0f));
}

void ArmorDector::SolveAngle(Armor& armor, const std::vector<cv::Point2f>& point2D, const std::vector<cv::Point3f>& point3D){
    cv::Mat rvecs = cv::Mat::zeros(3,1,CV_64FC1);
    cv::Mat tvecs = cv::Mat::zeros(3,1,CV_64FC1);
    double tx;
    double ty;
    double tz;

    cv::Mat caremaMatrix = contants::caremaMatrix_shoot;
    cv::Mat distCoeffs = contants::distCoeffs_shoot;
    cv::solvePnP(point3D, point2D, caremaMatrix, distCoeffs, rvecs, tvecs);  //解算x，y，z 三轴偏移量,将坐标换到以云台为轴的坐标系中

    tz = tvecs.ptr<double>(0)[2];
    tx = tvecs.ptr<double>(0)[0];
    ty = -tvecs.ptr<double>(0)[1];
    //tz = tvecs.ptr<double>(0)[2];

<<<<<<< HEAD

    armor.center3d_.x = (static_cast<float>(tx) + contants::kOffsetX )*10-58.58;
    armor.center3d_.y = (static_cast<float>(ty) + contants::kOffsetY)*10-61.07;
    armor.center3d_.z = (static_cast<float>(tz) + contants::kOffsetZ)*10;

    //armor.center3d_.x = 1;
    //cout << "??????????????????????????????????????" << armor.center3d_.x << endl;
=======
//-58.58
//-61.07
    armor.center3d_.x = (static_cast<float>(tx) + contants::kOffsetX )*10;
    armor.center3d_.y = (static_cast<float>(ty) + contants::kOffsetY)*10;
    armor.center3d_.z = (static_cast<float>(tz) + contants::kOffsetZ)*10;

>>>>>>> 这一版增加了数字识别，但效果不太理想
    armor.tx = tx;
    armor.ty = ty;
    armor.tz = tz;

//    if(display_mode)
//        cout << "装甲板据相机的距离为" << tz << endl;

}

Armor ArmorDector::Select_Best(int armor_num){
    Armor temp_armor;//考虑到距离近，面积肯定大，且极大几率不会偏，故忽略
<<<<<<< HEAD
=======
    cout << armor_num << endl;
>>>>>>> 这一版增加了数字识别，但效果不太理想
    int i,j;

    for (i=0; i<armor_num-1; ++i)
    {
        for (j=0; j<armor_num-1-i; ++j)
        {
            if (fabs(armor_array_[j].center2d_.x - (src_image_.cols)/2) + fabs(armor_array_[j].center2d_.y - (src_image_.rows)/2))
            {
                temp_armor = armor_array_[j];
                armor_array_[j] = armor_array_[j+1];
                armor_array_[j+1] = temp_armor;
            }
        }
    }

    best_armor = armor_array_[0];
    if( best_armor.tx != 0 && best_armor.ty != 0 && best_armor.tz != 0)best_armor.is_findTarget = 1;
<<<<<<< HEAD

=======
    circle(aultimate_image_,Point(best_armor.center2d_.x+50,best_armor.center2d_.y+50),3,Scalar(0, 255, 0));
    last_armor_ = best_armor;
>>>>>>> 这一版增加了数字识别，但效果不太理想
    return best_armor;
}

void ArmorDector::Continuous_Show(Armor best){
    if(display_mode == DisplayMode::Open)
    {
        line(aultimate_image_, best.left_led_[0], best.left_led_[1],Scalar(0, 0, 255), 2, 8);
        line(aultimate_image_, best.right_led_[0], best.left_led_[0],Scalar(0, 0, 255), 2, 8);
        line(aultimate_image_, best.right_led_[1], best.left_led_[1],Scalar(0, 0, 255), 2, 8);
        line(aultimate_image_, best.right_led_[1], best.right_led_[0],Scalar(0, 0, 255), 2, 8);
    }
    if(display_mode) imshow("armor", aultimate_image_);
}

