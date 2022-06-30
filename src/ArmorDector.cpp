#include "ArmorDector.h"
#include<vector>

int threshold_param = 222;

int element_size = 4;
int element_size1 = 5;

//#define display_mode DisplayMode::Open
//#define display_mode DisplayMode::Close

#define color_ Color::Blue
//#define color_  Color::Red
//int i = 0;
//int max1 = 255;
unsigned short ArmorDector::StorePicture(Mat &img)
{
    //image_rows_ = img.rows;
    //image_cols_ = img.cols;

    // if(last_is_have_target_)
    // {
    //     src_image_ = img(roi).clone();
    // }
    // else
    // {
    //     src_image_ = img;
    // }
    src_image_ = img;

    // 耗时0.3-0.8ms不等
    if(display_mode) show_image_ = src_image_.clone();

    aultimate_image_ = img;
    SeparateColor();
    led_num = GetAllLed();
    armor_num_ = GetAllArmor(led_num);
    AngleResolving(armor_num_);

    Getbest();

    if(armor_num_)
    {
        last_is_have_target_ = true;
    }
    else
    {
        last_is_have_target_ = false;
    }

    return armor_num_;
}


void ArmorDector::SeparateColor(){


    Mat splited_image[3];

    // 这个地方有最多2ms的耗时，主要耗时在1.3ms左右
//    split(src_image_, splited_image);

//    // 这个地方有最多2ms的耗时，主要耗时在1.3ms左右
//    if(color_ == Color::Blue)
//        channel_image_ = splited_image[0] - 0.7f*splited_image[2];//蓝色
//    else channel_image_ = splited_image[2] - 0.7f*splited_image[0];//红色

    cvtColor(src_image_,channel_image_,cv::COLOR_RGB2GRAY);
    // 最多耗时0.12ms-0.14ms左右
    threshold(channel_image_, channel_image_, threshold_param, 255, THRESH_BINARY);   

    namedWindow("threshold", WINDOW_AUTOSIZE);
    createTrackbar("threshold", "threshold", &threshold_param, 255);

    // 下面四步耗时0.5-0.7ms
    //Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(element_size, element_size));
    //Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(element_size1, element_size1));

    //dilate(channel_image_, channel_image_, element1);

    //erode(channel_image_, channel_image_, element);

    
    final_image_ = channel_image_;

    //createTrackbar("element_size", "threshold", &element_size,12);
    //createTrackbar("element_size1", "threshold", &element_size1,12);



    if(display_mode == DisplayMode::Open)
    {
        string windowName = "处理后的图片";
        namedWindow(windowName, 0);
        imshow(windowName,final_image_);
    }

    //chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    //chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
    //waitKey(10);

    //cout << "time_used_for_identify:    " << (time_used.count())*1000 << endl;

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

        if(area < 20 || area > 20000) continue;//5 100

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

        auto rect = led_rect.boundingRect();
        int sum_r = 0, sum_b = 0;
        if(rect.x < 0)
        {
            rect.x = 0;
        }
        if(rect.y < 0)
        {
            rect.y = 0;
        }
        if((rect.x + rect.width) > src_image_.cols)
        {
            rect.width = src_image_.cols - rect.x;
        }
        if((rect.y + rect.height) > src_image_.rows)
        {
            rect.height = src_image_.rows - rect.y;
        }
        auto roi = src_image_(rect);
        for (int i = 0; i < roi.rows; i++) {
          for (int j = 0; j < roi.cols; j++) {
            if (cv::pointPolygonTest(led_contour, cv::Point2f(j + rect.x, i + rect.y), false) >= 0) {
              // if point is inside contour
              sum_b += roi.at<cv::Vec3b>(i, j)[0];
              sum_r += roi.at<cv::Vec3b>(i, j)[2];
            }
          }
        }
        if(color_ == Color::Blue)// need to indentify bule armor
        {
            if(sum_r > sum_b)// the fact armor is red armor
            {
                continue;
            }
        }
        else// need to indentify red armor
        {
            if(sum_b > sum_r)// the fact armor is blue armor
            {
                continue;
            }
        }

        led_array_[led_num++] = led;


        if(display_mode == DisplayMode::Open)
        {
            line(show_image_, topLeft, topRight,Scalar(255, 0, 255), 2, 8);
            line(show_image_, topLeft, bottomLeft,Scalar(255, 0, 255), 2, 8);
            line(show_image_, bottomRight, topRight,Scalar(255, 0, 255), 2, 8);
            line(show_image_, bottomRight, bottomLeft,Scalar(255, 0, 255), 2, 8);
        }

    }
cout << "*************************************" << endl;
    if(display_mode == DisplayMode::Open)
    {
        // 大概有6-7ms耗时
         string windowName = "led";
         namedWindow(windowName, 0);
         imshow(windowName, show_image_);
        
    }
    waitKey(1);
    //if(display_mode)cout<<"灯条数目为"<<led_num<<endl;
    return led_num;
}

unsigned short ArmorDector::GetAllArmor(int led_num)
{
    chrono::steady_clock::time_point ta = chrono::steady_clock::now();
    unsigned short armor_num = 0;
    bool is_selected[led_num];

    memset(is_selected, false, led_num);
    sort(led_array_, led_array_ + led_num, ledCmpLength);


    for(unsigned short i = 0; i < led_num; i++)
    {
        for(unsigned short index = i+1; index < led_num; index++){

            //if(is_selected[i] || is_selected[index]) continue; //未被配对

            float lRatio = led_array_[i].length_ / led_array_[index].length_; //长度相近

            if(lRatio > 1.25f || lRatio < 0.75f)
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

            if(std::abs(AngleMinus) > 10.0f)
            {
                cout << "角度不符合" << endl;
                continue;
            }


            is_selected[i] = true;
            is_selected[index] = true;
            Armor armor;
            armor.left_led_[0] = led_array_[i].top_point_;
            armor.left_led_[1] = led_array_[i].bottom_point_;
            armor.right_led_[0] = led_array_[index].top_point_;
            armor.right_led_[1] = led_array_[index].bottom_point_;

            armor.center2d_.x = (armor.left_led_[0].x + armor.right_led_[1].x)*0.5;
            armor.center2d_.y = (armor.left_led_[0].y + armor.right_led_[1].y)*0.5;

            float width_height_ratio = std::sqrt((std::pow((armor.left_led_[0].x - armor.left_led_[1].x),2)) + pow((armor.left_led_[0].y - armor.left_led_[1].y),2)) 
                                        / std::sqrt(std::pow(armor.left_led_[0].x-armor.right_led_[0].x ,2) + std::pow(armor.left_led_[0].y - armor.right_led_[0].y ,2));

            cout << "长宽比:    " <<  width_height_ratio << endl;
            
            // 左大，右小
            if(!((width_height_ratio > 0.42 && width_height_ratio < 0.48) || (width_height_ratio > 0.22 && width_height_ratio < 0.33)))
            {
                cout << "长宽比例不符合" << endl;
                //continue;
            }

            float a = std::sqrt((std::pow((armor.left_led_[0].x - armor.left_led_[1].x),2)) + pow((armor.left_led_[0].y - armor.left_led_[1].y),2));
            float b = std::sqrt((std::pow((armor.right_led_[0].x - armor.left_led_[0].x),2)) + std::pow((armor.right_led_[0].y - armor.left_led_[0].y),2));
            float c = std::sqrt((std::pow(armor.right_led_[0].x-armor.left_led_[1].x,2))+std::pow(armor.right_led_[0].y-armor.left_led_[1].y,2));

            float angleHeightWidthRatio1 = std::acos((a*a + c*c - b*b)/(2*a*c))*180/CV_PI;
            float angleHeightWidthRatio2 = std::acos((c*c + b*b - a*a)/(2*c*b))*180/CV_PI;
            float angleHeightWidthRatio =  180 - (angleHeightWidthRatio2 + angleHeightWidthRatio1);

            //cout << "armor angle roll is " << angleHeightWidthRatio << endl;

            if(angleHeightWidthRatio > 105 || angleHeightWidthRatio < 75)
            {
                //cout << "装甲板矩形不符合标准" << endl;
                continue;
            }

            float e = std::sqrt(std::pow((armor.right_led_[0].x - armor.right_led_[1].x),2) + std::pow((armor.right_led_[0].y - armor.right_led_[1].y),2));
            float f = std::sqrt(std::pow((armor.right_led_[1].x - armor.left_led_[1].x),2) + std::pow((armor.right_led_[1].y - armor.left_led_[1].y),2));
            float g = std::sqrt(std::pow((armor.right_led_[0].x - armor.left_led_[1].x),2) + std::pow((armor.right_led_[0].y - armor.left_led_[1].y),2));

            float angleHeightWidthRatio_1 = std::acos((f*f + g*g - e*e)/(2*f*g))*180/CV_PI;
            float angleHeightWidthRatio_2 = std::acos((e*e + g*g - f*f)/(2*e*g))*180/CV_PI;
            float angleHeightWidthRatio_ = 180 - (angleHeightWidthRatio_1 + angleHeightWidthRatio_2);

            if(angleHeightWidthRatio_ > 105 || angleHeightWidthRatio_ < 75)
            {
                //cout << "装甲板矩形不符合标准" << endl;
                continue;
            }

            if(led_array_[i].center_.x > led_array_[index].center_.x)
            {
                swap(armor.left_led_, armor.right_led_);
            }


            //ROI = aultimate_image_(roi);
            chrono::steady_clock::time_point tc = chrono::steady_clock::now();

            // 0.3-0.5ms
            //armor.id = GetNumber(aultimate_image_,armor.center2d_,std::sqrt((std::pow((armor.left_led_[0].x - armor.left_led_[1].x),2)) + pow((armor.left_led_[0].y - armor.left_led_[1].y),2)) ,atan(led_array_[i].slope_)*180/CV_PI);
            
            //armor.id = hog_svm.predictID(aultimate_image_,armor.center2d_,std::sqrt((std::pow((armor.left_led_[0].x - armor.left_led_[1].x),2)) + pow((armor.left_led_[0].y - armor.left_led_[1].y),2)) ,atan(led_array_[i].slope_)*180/CV_PI);

            armor.id = nc.doClassify(aultimate_image_,armor.center2d_,std::sqrt((std::pow((armor.left_led_[0].x - armor.left_led_[1].x),2)) + pow((armor.left_led_[0].y - armor.left_led_[1].y),2)) ,atan(led_array_[i].slope_)*180/CV_PI);
            cout << "装甲板数字：   " << armor.id << endl;

            chrono::steady_clock::time_point tb = chrono::steady_clock::now();
            chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(tb - tc);
            cout << "time_used_storePiucure:    " << time_used.count()*1000 << endl; 

            cout << armor.id << endl;

            if(armor.id == 0 || armor.id == -1)
            {
                continue;
            }

            //armor.width = centerDistance;
            
            if(width_height_ratio > 0.40 && width_height_ratio < 0.50)
            {
                cout << "small" << endl;
                armor.armor_type_ = Armor::ArmorType::SMALL;
            }
            else if(width_height_ratio > 0.20 && width_height_ratio < 0.30)
            {
                cout << "big" << endl;
                armor.armor_type_ = Armor::ArmorType::SMALL;
            }

            if(armor.armor_type_ == Armor::ArmorType::SMALL && armor.id == 1)
            {
                continue;
            }

            //armor.armor_type_ = Armor::ArmorType::LARGE;
            
            armor.center2d_.x = ( armor.left_led_[0].x + armor.right_led_[1].x)/2;
            armor.center2d_.y = ( armor.left_led_[0].y + armor.right_led_[1].y)/2;
            armor.width_ = b;
            armor.height_ = a;
            armor_array_[armor_num] = armor;

            height_of_led_ = a;

            cout << "led _" << a << endl;
            //cout << armor_array_[armor_num].center2d_.x << "    " << armor_array_[armor_num].center2d_.y << endl;

            UpdateRoi(armor,image_cols_,image_rows_);

            rectangle(aultimate_image_,roi,Scalar(50,100,200),4,7);

            if(display_mode == DisplayMode::Open)
            {
                line(aultimate_image_, armor.left_led_[0], armor.left_led_[1],Scalar(193, 182, 255), 1, 8);
                line(aultimate_image_, armor.right_led_[0], armor.left_led_[0],Scalar(193, 182, 255), 1, 8);
                line(aultimate_image_, armor.right_led_[1], armor.left_led_[1],Scalar(193, 182, 255), 1, 8);
                line(aultimate_image_, armor.right_led_[1], armor.right_led_[0],Scalar(193, 182, 255), 1, 8);

                //rectangle(aultimate_image_,roi,Scalar(50,100,200),4,7);
                char test[100];
                sprintf(test, "%d", armor.id);
                cv::putText(aultimate_image_, test, cv::Point(armor.center2d_.x, armor.center2d_.y-(a*0.5)), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8);

                if(armor.armor_type_ == Armor::ArmorType::SMALL)
                {
                    circle(aultimate_image_,Point(armor_array_[armor_num].center2d_.x,armor_array_[armor_num].center2d_.y),3,Scalar(255, 255, 255));
                }
                else
                {
                    circle(aultimate_image_,Point(armor_array_[armor_num].center2d_.x,armor_array_[armor_num].center2d_.y),3,Scalar(255, 0, 255));
                }
            }
            armor_num++;

        }
        //if(armor_num == ARMOR_SIZE) break;
    }

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
    //waitKey(10);

    //cout << "time_used_for_identify:    " << (time_used.count())*1000 << endl;

    return armor_num;

}


void ArmorDector::AngleResolving(const unsigned short& armor_num)
{

    vector<cv::Point2f> image_coodinate_points;
    vector<cv::Point3f> object_coodinate_points;

    for(unsigned short index = 0; index < armor_num; index++){
        GetImagePointData(armor_array_[index], image_coodinate_points);
        GetObjectPointData(armor_array_[index], object_coodinate_points);
        SolveAngle(armor_array_[index], image_coodinate_points, object_coodinate_points);
    }

    if(armor_num == 1)
    {
        best_armor = armor_array_[0];
        //cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << best_armor.center3d_.x << endl;
        best_armor.is_findTarget = 1;

        last_armor_ = best_armor;
    }
    else if(armor_num == 0)
    {
        best_armor = last_armor_;
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


//-58.58
//-61.07
    // armor.center3d_.x = (static_cast<float>(tx) + contants::kOffsetX )*10 - 26.48 - 70.00;
    // armor.center3d_.y = (static_cast<float>(ty) + contants::kOffsetY)*10 + 42.53;
    // armor.center3d_.z = (static_cast<float>(tz) + contants::kOffsetZ)*10 + 41.29;

    armor.center3d_.x = (static_cast<float>(tx) + contants::kOffsetX )/100 ;
    armor.center3d_.y = (static_cast<float>(ty) + contants::kOffsetY)/100 + 0.045f;
    armor.center3d_.z = (static_cast<float>(tz) + contants::kOffsetZ)/100 + 0.132f;


    armor.tx = armor.center3d_.x;
    armor.ty = armor.center3d_.y;
    armor.tz = armor.center3d_.z;

//    if(display_mode)
//        cout << "装甲板据相机的距离为" << tz << endl;

}

Armor ArmorDector::Select_Best(int armor_num){
    Armor temp_armor;//考虑到距离近，面积肯定大，且极大几率不会偏，故忽略

    cout << armor_num << endl;

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

    circle(aultimate_image_,Point(best_armor.center2d_.x+50,best_armor.center2d_.y+50),3,Scalar(0, 255, 0));
    last_armor_ = best_armor;

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

