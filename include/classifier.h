// Copyright 2022 Chen Jun

#ifndef ARMOR_DETECTOR__NUMBER_CLASSIFIER_HPP_
#define ARMOR_DETECTOR__NUMBER_CLASSIFIER_HPP_

// STL
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "common.h"

using namespace Horizon;

class NumberClassifier
{
public:
  NumberClassifier()
  {
      net_ = cv::dnn::readNetFromONNX("model_path");
      threshold = 0.5;
      std::ifstream label_file("label_path");
      std::string line;
      while (std::getline(label_file, line)) {
        class_names_.push_back(line[0]);
      }
   }

  //void extractNumbers(const cv::Mat & src, std::vector<Armor> & armors);

  char doClassify(Mat &Src,Point2f center,float height ,float angle)
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

      Mat armor_roi = Src(Rect(x_left,y_on,x_right - x_left,y_down - y_on));

      if(angle>5||angle<175){
          Mat rot_mat = getRotationMatrix2D(Point(armor_roi.cols/2,armor_roi.cols/2), -angle, 1.0);//求旋转矩阵
          Size dst_sz(armor_roi.cols,armor_roi.rows);
          warpAffine(armor_roi, armor_roi, rot_mat, dst_sz,INTER_NEAREST);//原图像旋转
      }

      Mat src_num = armor_roi(Rect(armor_roi.cols/5,0,armor_roi.cols*3/5,armor_roi.rows));

      cv::cvtColor(src_num, src_num, cv::COLOR_RGB2GRAY);
      cv::threshold(src_num, src_num, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

      cv::Mat image = src_num;

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

      if(confidence > 0.95)
      {
          number = class_names_[label_id];
      }
      else
      {
          number = '0';
      }

      //char number = class_names_[label_id];

      return number;

  }

  double threshold;

private:
  cv::dnn::Net net_;
  std::vector<char> class_names_;
};















// namespace rm_auto_aim

#endif  // ARMOR_DETECTOR__NUMBER_CLASSIFIER_HPP_
