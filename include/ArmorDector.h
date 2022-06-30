#ifndef ARMORDECTOR_H
#define ARMORDECTOR_H
#include <opencv2/opencv.hpp>
#include<iostream>

#include "common.h"
#include "constants.h"
#include <opencv2/ml.hpp>
#include "common.h"
#include "constants.h"
//#include "classifier.h"
#include "GetNum.h"
#include "bp.h"

using namespace Horizon;
using namespace cv;
using namespace std;

//int threshold_1 = 0;
class Led{
public:
    Led()
       :
       slope_(0), length_(0), width_(0),ratio_(0),   //初始化斜度，长度，宽度，比率，上顶点，下顶点，中心点
       top_point_(), bottom_point_(), center_()
    {};

    ~Led(){};

public:
    float slope_;
    float length_;
    float width_;
    float ratio_;
    Point2f center_;
    Point2f top_point_;
    Point2f bottom_point_;

};

const string FILENAME =  "../tools/para/";
const string HOG_SVM_FILE = "../svmTrainFile/HOG_SVM.xml";

/* hog-svm定义 */
class HOG_SVM
{
private:
    cv::Ptr<cv::ml::SVM> m_svm;
    std::map<int, int> m_label2id;
    cv::HOGDescriptor m_hog;
    

public:
    HOG_SVM()
    {
        m_label2id = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 11}, {7, 7}, {8, 8}};
        m_hog.winSize = Size(48, 32);
        m_hog.blockSize = Size(16, 16);
        m_hog.blockStride = Size(8, 8);
        m_hog.cellSize = Size(8, 8);
        m_hog.nbins = 9;
        m_hog.derivAperture = 1;
        m_hog.winSigma = -1;
        m_hog.histogramNormType = HOGDescriptor::L2Hys;
        m_hog.L2HysThreshold = 0.2;
        m_hog.gammaCorrection = false;
        m_hog.free_coef = -1.f;
        m_hog.nlevels = HOGDescriptor::DEFAULT_NLEVELS;
        m_hog.signedGradient = false;
        if (m_svm)
        {
            m_svm->clear();
        }
        m_svm = cv::ml::SVM::load("/home/liqianqi/RMCV2022/include/HOG_SVM.xml");
    }

    int test(const Mat &src)
    {
        if(!m_svm.empty())
        {
            cout << "SVM success!" << endl;
            vector<float> descriptors;
            m_hog.compute(src, descriptors, Size(8, 8));
            int label = m_svm->predict(descriptors);
            return m_label2id[label];
        }
        else
        {
            cout << "SVM false!" << endl;
            return 0;
        }
    }

    int predictID(Mat &Src,Point2f center,float height ,float angle)
    {
        float y_on = center.y - height;
        float y_down = center.y + height;

        float x_left = center.x - 0.7*height;
        float x_right = center.x + 0.7*height;

        if(angle>90)
        {
            angle = -(180 - angle);
        }
        if(y_on<0)
        {
            y_on = 0;
        }
        if(y_down>Src.rows)
        {
            y_down = Src.rows;
        }
        if(x_left<0)
        {
            x_left = 0;
        }
        if(x_right > Src.cols)
        {
            x_right = Src.cols;
        }

        Mat armor_roi = Src(Rect(x_left,y_on,x_right - x_left,y_down - y_on));

        if(angle>5 || angle<175)
        {
            Mat rot_mat = getRotationMatrix2D(Point(armor_roi.cols/2,armor_roi.cols/2), -angle, 1.0);//求旋转矩阵
            Size dst_sz(armor_roi.cols,armor_roi.rows);
            warpAffine(armor_roi, armor_roi, rot_mat, dst_sz,INTER_NEAREST);//原图像旋转
        }
        //armor_roi = armor_roi(Rect(armor_roi.cols/5,0,armor_roi.cols*3/5,armor_roi.rows));

        Mat tmp;
        resize(armor_roi, tmp, Size(48, 32));
        cvtColor(tmp, tmp, COLOR_RGB2GRAY);
        // threshold(tmp,tmp,20,255,CV_THRESH_BINARY | CV_THRESH_OTSU);

        Rect front_roi(Point(20, 0), Size(10, 32));
        Mat front_roi_img = tmp(front_roi);
        
        //namedWindow("num_", WINDOW_AUTOSIZE);
        //createTrackbar("threshold", "num_", &threshold_1, 255);

        // double min = 0, max = 0;
        // minMaxLoc(front_roi_img, &min, &max);
        // tmp = tmp * (255.0 / max);

        if(armor_roi.empty())
        {
            cout << "empty!" << endl;
        }
        else
        {
            imshow("1",tmp);
        }

        int ID = test(tmp);

        return ID;

    }

};

class NumberClassifier
{
public:
  NumberClassifier()
  {
      net_ = cv::dnn::readNetFromONNX("/home/liqianqi/RMCV2022/include/fc.onnx");
      threshold = 0.5;
      std::ifstream label_file("/home/liqianqi/RMCV2022/include/label.txt");
      std::string line;
      while (std::getline(label_file, line)) {
        class_names_.push_back(line[0]);
      }
   }

  //void extractNumbers(const cv::Mat & src, std::vector<Armor> & armors);

  int doClassify(Mat &Src,Point2f center,float height ,float angle)
  {

      float y_on = center.y - height;
      float y_down = center.y  + height;

      float x_left = center.x - height;
      float x_right = center.x + height;

      if(angle>90){
          angle = -(180 - angle);
      }

      if(y_on<0){
          y_on = 0;
      }
      if(y_down>Src.rows){
          y_down = Src.rows;
      }
      if(x_left<0){
          x_left = 0;
      }
      if(x_right > Src.cols){
          x_right = Src.cols;
      }

      Mat armor_roi = Src(Rect(x_left,y_on,x_right - x_left,y_down - y_on)).clone();

      if(angle>5||angle<175){
          Mat rot_mat = getRotationMatrix2D(Point(armor_roi.cols/2,armor_roi.cols/2), -angle, 1.0);//求旋转矩阵
          Size dst_sz(armor_roi.cols,armor_roi.rows);
          warpAffine(armor_roi, armor_roi, rot_mat, dst_sz,INTER_NEAREST);//原图像旋转
      }

      Mat src_num = armor_roi(Rect(armor_roi.cols/5,0,armor_roi.cols*3/5,armor_roi.rows));

      cv::cvtColor(src_num, src_num, cv::COLOR_RGB2GRAY);
      cv::threshold(src_num, src_num, 150, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

      cv::Mat image = src_num;

      if(armor_roi.empty())
      {
          cout << "empty!" << endl;
      }
      else
      {
          imshow("number",image);
      }


      // Normalize
      image = image / 255.0;


      // Create blob from image
      cv::Mat blob;
      cv::dnn::blobFromImage(image, blob, 1., cv::Size(28, 20));

      // Set the input blob for the neural network
      net_.setInput(blob);
      // Forward pass the image blob through the model
      cv::Mat outputs = net_.forward();

      // Do softmax
      float max_prob = *std::max_element(outputs.begin<float>(), outputs.end<float>());
      cv::Mat softmax_prob;
      cv::exp(outputs - max_prob, softmax_prob);
      float sum = static_cast<float>(cv::sum(softmax_prob)[0]);
      softmax_prob /= sum;

      double confidence;
      cv::Point class_id_point;
      minMaxLoc(softmax_prob.reshape(1, 1), nullptr, &confidence, nullptr, &class_id_point);
      int label_id = class_id_point.x;

      //armor.confidence = confidence;
      char number;

      if(confidence > 0.5)
      {
          number = class_names_[label_id];
      }
      else
      {
          number = '0';
      }

      //char number = class_names_[label_id];

      int number_;
      switch(number)
      {
          case '1':
              number_ = 1;
              break;
          case '2':
              number_ = 2;
              break;
          case '3':
              number_ = 3;
              break;
          case '4':
              number_ = 4;
              break;
          case '5':
              number_ = 5;
              break;
          case 'O':
              number_ = 6;
              break;
          case 'G':
              number_ = 7;
              break;
          case 'B':
              number_ = 8;
              break;
          case 'N':
              number_ = 0;
              break;
          default:
              number_ = 0;
              break;
      }

      return number_;

  }

  double threshold;

private:
  cv::dnn::Net net_;
  std::vector<char> class_names_;
};


class ArmorDector
{
public:
    ArmorDector()
        //classifier(FILENAME)
    {
        int cols = 640;
        int rows = 512;
        InitRoi(cols,rows);

        //bp.load("../trainData/model_sigmoid_800_200.xml");
    }

    unsigned short StorePicture(Mat &img);
    void SeparateColor();
    unsigned short GetAllLed();
    unsigned short GetAllArmor(int led_num);
    Armor Select_Best(int armor_num);
    void Continuous_Show(Armor best);

    void ObtainAllArmor(vector<Armor> All_Armor);

    void AngleResolving(const unsigned short& armor_num);
    void GetImagePointData(const Armor& allArmor, std::vector<cv::Point2f>& point2D);
    void GetObjectPointData(const Armor& armor, std::vector<cv::Point3f>& point3D);
    void SolveAngle(Armor& armor, const std::vector<cv::Point2f>& point2D, const std::vector<cv::Point3f>& point3D);
    
    /**
     * @brief 获得的装甲板传给预测器中
     * 
     * @return 把三维坐标传出去
     */

    Point3f Getbest(){

        Point3f best_point;
        best_point.x = best_armor.center3d_.x;
        best_point.y = best_armor.center3d_.y;
        best_point.z = best_armor.center3d_.z;
        return best_point;

    }; 

    enum ArraySize{
        LED_SIZE = 20,
        ARMOR_SIZE = 10
    };

private:
    Mat src_image_;
    Mat binary_image_;
    Mat channel_image_;
    Mat aultimate_image_;
    Mat final_image_;
    Mat show_image_;
    Mat ROI;

    HOG_SVM hog_svm;

    enum Color{Blue = 1,Red = 0};

    unsigned short led_num;

    Point2f pigment_poi[9];              //储存对应坐标
    Led led_array_[LED_SIZE];            //存储led
    Armor armor_array_[ARMOR_SIZE];      //存储装甲板
    Armor select_best( int armor_num);

    int image_cols_;
    int image_rows_;

public:

    bool last_is_have_target_ = false;
public:
   Armor best_armor;                    // 存储最优装甲板
   Armor last_armor_;                   // 上一帧的装甲板
   unsigned short armor_num_;

public:
    NumberClassifier nc;             // CNN分类器，用于数字识别
    bp::Net bp;                                                    // bp神经网络

    // visual led
    float height_of_led_;

public:
    //Ptr<TrackerKCF> tracker = TrackerKCF::create();        //跟踪器

    Rect roi;
    cv::Mat armorRoi;
    Rect2d bbox;

    int getArmorID(cv::Mat &Src,Point2f center,float height ,float angle)
    {
        float y_on = center.y - height;
        float y_down = center.y  + height;

        float x_left = center.x - height;
        float x_right = center.x + height;

        if(angle>90){
            angle = -(180 - angle);
        }

        if(y_on<0){
            y_on = 0;
        }
        if(y_down>Src.rows){
            y_down = Src.rows;
        }
        if(x_left<0){
            x_left = 0;
        }
        if(x_right > Src.cols){
            x_right = Src.cols;
        }

        armorRoi = Src(Rect(x_left,y_on,x_right - x_left,y_down - y_on));

        if(angle>5||angle<175){
            Mat rot_mat = getRotationMatrix2D(Point(armorRoi.cols/2,armorRoi.cols/2), angle, 1.0);//求旋转矩阵
            Size dst_sz(armorRoi.cols,armorRoi.rows);
            warpAffine(armorRoi, armorRoi, rot_mat, dst_sz,INTER_NEAREST);//原图像旋转
        }

        armorRoi = armorRoi(Rect(armorRoi.cols/5,0,armorRoi.cols*3/5,armorRoi.rows));


        cv::Mat grayResizeImg;
        cv::cvtColor(armorRoi, grayResizeImg, cv::COLOR_RGB2GRAY);
        cv::Mat predict_img = cv::Mat(400, 1, CV_32FC1);
        predict_img = bp.bpImgPreProcess(grayResizeImg);

        int response = bp.predict_one(predict_img);


        return response;

    }

    void InitRoi(int cols,int rows)
    {
        roi = Rect(0,0,cols,rows);
    }

    void UpdateRoi(Armor & armor,int cols,int rows)
    {
        Point2f left_on = Point2f(armor.center2d_.x,armor.center2d_.y) - Point2f(armor.width_,armor.height_*2);
        Point2f right_on = Point2f(armor.center2d_.x,armor.center2d_.y) + Point2f(armor.width_,armor.height_*2);

        if(left_on.x < 0)
        {
            left_on.x = 0;
        }
        if(left_on.y < 0)
        {
            left_on.y = 0;
        }
        if(right_on.x > cols)
        {
            right_on.x = cols;
        }
        if(right_on.y > rows)
        {
            right_on.y = rows;
        }

        roi = Rect(armor.center2d_.x - 2*armor.width_,armor.center2d_.y - 2*armor.height_,4*armor.width_,4*armor.height_);
        bbox = Rect2d(roi);

    }
public://排列组合函数区

    static bool ledCmpLength(const Led a, const Led b)     //长的优先
    {
        return a.length_ > b.length_;
    }

    static bool ledCmpSlope(const Led a, const Led b)      //竖直优先
    {
        return a.length_ < b.length_;
    }

    static bool ledCmpY(const Point2f a, const Point2f b)  //y优先
    {
        return a.y < b.y;
    }

    static bool ledCmpX(const Point2f a, const Point2f b)  //x优先
    {
        return a.x < b.x;
    }

};


#endif // ARMORDECTOR_H
