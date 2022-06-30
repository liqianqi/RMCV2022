#include "common.h"

namespace bp 
{
    class Net 
    {
    public:
        Net() {};
        ~Net() {};
        int predict_one(cv::Mat &input);
        void load(const std::string &filename);  //导入训练的xml文件
        static cv::Mat bpImgPreProcess(cv::Mat input);//对图片进行预处理
        std::vector<int> layerNeuronNum;		//网络的结构
        std::string activeFunction = "tanh";	//选择tanh激活函数
        float learningRate;						//学习率(分类时不用使用，只在训练的时候使用)
    private:
        int _resultNumber;						//分类的结果
        void forward();							//前向传播
        void initNet(std::vector<int> layer_neuron_num_); //通过XML文件初始化bp模型
    protected:
        std::vector<cv::Mat> layer;
        std::vector<cv::Mat> weights;
        std::vector<cv::Mat> bias;
        cv::Mat activationFunction(cv::Mat &x, const std::string &func_type);
        static cv::Mat sigmoid(cv::Mat &x);		//sigmoid激活函数
        static cv::Mat tanh(cv::Mat &x);		//tanh激活函数
    };
}