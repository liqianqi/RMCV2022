#ifndef KALAMAN
#define KALAMAN
#include"common.h"

namespace Horizon{

#define MAX_TIME_BIAS 1000
class Kalman_Filter {
public:
    Kalman_Filter() {
        _initFlag = false;
        _jumpLimit.x = 500;
        _jumpLimit.y = 200;
        _jumpLimit.z = 500;
        _maxLimit.x = 8000;
        _maxLimit.y = 1000;
        _maxLimit.z = 8000;
        InitParamOthers();
    }

    ~Kalman_Filter() {}
    void changeParamQ_R(){

        namedWindow("kalaman_R", CV_WINDOW_AUTOSIZE);
        createTrackbar("Rx", "kalaman_R", &Rx,max);
        namedWindow("kalaman_R", CV_WINDOW_AUTOSIZE);
        createTrackbar("Ry", "kalaman_R", &Ry,max);
        namedWindow("kalaman_R", CV_WINDOW_AUTOSIZE);
        createTrackbar("Rz", "kalaman_R", &Rz,max);

        Eigen::MatrixXd VinputR(3, 3);		// 初始化观测噪声矩阵,第一维是x的测量误差，第二维是y的测量误差，第三维是z的测量误差。
        VinputR <<	Rx, 0.0, 0.0,
                    0.0, Ry, 0.0,
                    0.0, 0.0, Rz;

        namedWindow("kalaman_Q", CV_WINDOW_AUTOSIZE);
        createTrackbar("Qx", "kalaman_Q", &Qx,max);
        namedWindow("kalaman_Q", CV_WINDOW_AUTOSIZE);
        createTrackbar("Qy", "kalaman_Q", &Qy,max);
        namedWindow("kalaman_Q", CV_WINDOW_AUTOSIZE);
        createTrackbar("Qz", "kalaman_Q", &Qz,max);
        namedWindow("kalaman_Q", CV_WINDOW_AUTOSIZE);
        createTrackbar("Qvx", "kalaman_Q", &Qvx,max);
        namedWindow("kalaman_Q", CV_WINDOW_AUTOSIZE);
        createTrackbar("Qvy", "kalaman_Q", &Qvy,max);
        namedWindow("kalaman_Q", CV_WINDOW_AUTOSIZE);
        createTrackbar("Qvz", "kalaman_Q", &Qvz,max);

        Eigen::MatrixXd VinputQ(6, 6);      // 初始化过程噪声矩阵，分别是x,y,z,vx,vy,vz的预测的置信度，越大置信度越低。
        VinputQ <<	Qx, 0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, Qy, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, Qz, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, Qvx, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, Qvy, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0, Qvz;

        SetQ(VinputQ);
        SetR(VinputR);

    }
    //初始化卡尔曼参数（状态转移矩阵中含有时间间隔，需要随迭代更新）
    void InitParam(Eigen::MatrixXd inputP, Eigen::MatrixXd inputQ, Eigen::MatrixXd inputH, Eigen::MatrixXd inputR) {
        SetP(inputP);

        SetQ(inputQ);

        SetH(inputH);

        SetR(inputR);
    }
// invalid operands to binary expression('Eigen::MatrixXd'(aka'Matrix<double,Dynamic,Dynamic>')and 'double')
    void InitParamOthers(){
        //初始化状态协方差矩阵
        Eigen::MatrixXd VinputP(6, 6);
        VinputP <<  1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 100.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 100.0, 0.0,
                    0.0, 0.0, 0.0, 1.0, 0.0, 100.0;

		Eigen::MatrixXd VinputQ(6, 6);		//初始化过程噪声矩阵
        VinputQ <<	7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 7.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 200.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 5.0, 0.0,
                    0.0, 0.0, 0.0, 0.0, 0.0, 200.0;

		Eigen::MatrixXd VinputH(3, 6);		//初始化观测矩阵
		VinputH <<	1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0, 0.0, 0.0;

		Eigen::MatrixXd VinputR(3, 3);		//初始化观测噪声矩阵
        VinputR <<	1000.0, 0.0, 0.0,
                    0.0, 1000.0, 0.0,
                    0.0, 0.0, 1000.0;
        InitParam(VinputP,VinputQ,VinputH,VinputR);
    }

    void setLimit(cv::Point3f maxLimit, cv::Point3f jumpLimit) {
        _maxLimit = maxLimit;
        _jumpLimit = jumpLimit;
    }

    Point3f GetX() {
        return Point3f(_x[0],_x[1],_x[2]);
    }

    bool isInitialized() {
        return _initFlag;
    }

    void Initialization(Eigen::VectorXd inputX) {
        _x = inputX;
        _initFlag = true;
    }

    void InitializationX(Eigen::VectorXd inputX) {
        _x = inputX;
    }

    void SetF(Eigen::MatrixXd inputF) {
        _f = inputF;
    }

    void SetP(Eigen::MatrixXd inputP) {
        _p = inputP;
    }

    void SetQ(Eigen::MatrixXd inputQ) {
        _q = inputQ;
    }

    void SetH(Eigen::MatrixXd inputH) {
        _h = inputH;
    }

    void SetR(Eigen::MatrixXd inputR) {
        _r = inputR;
    }

    void setInitFalse() {
        _initFlag = false;
    }

    /**
     * 卡尔曼滤波
     */
    void Prediction();

    /**
     * 更新测量值
     * @param z 测量值
     */
    void MeasurementUpdate(const Eigen::VectorXd& z);

    /**
     * 卡尔曼滤波主函数
     * @param originData 原始数据
     */
    cv::Point3f KPredictionRun(cv::Point3f &originData);

    /**
     * 剔除异常数据
     * @param originData 原始数据
     */
    cv::Point3f wrongDataKiller(cv::Point3f &originData);
    /**
     * 用于调用预判与观测函数
     */
<<<<<<< HEAD
    cv::Point3f KFilter(cv::Point3f &coordinate);
=======
    cv::Point3f KFilter(const cv::Point3f &coordinate);
>>>>>>> 这一版增加了数字识别，但效果不太理想

    /**
    * 获取当前坐标对应的时间（ms）
    */
    float getKFtime() {
        return _nowTime_ms;
    }

private:

    bool _initFlag = false;													//初始化标志位
    int _badCntC = 0;
    float _deltaTime_ms = 0.0f, _nowTime_ms = 0.0f, _lastTime_ms = 0.0f;		//单位ms
    cv::Point3f _lastOriginData;
    cv::Point3f _maxLimit;
    cv::Point3f _jumpLimit;
    Eigen::VectorXd _x;		//状态矩阵包含 位置、速度
    Eigen::MatrixXd _f;		//状态转移矩阵
    Eigen::MatrixXd _p;		//状态协方差矩阵
    Eigen::MatrixXd _q;		//过程噪声矩阵
    Eigen::MatrixXd _h;		//测量矩阵
    Eigen::MatrixXd _r;		//测量噪声矩阵

    int max = 10000;

    int Rx = 1000.0;
    int Ry = 1000.0;
    int Rz = 1000.0;

    int Qx = 7.0;
    int Qy = 1.0;
    int Qz = 7.0;
    int Qvx = 200.0;
    int Qvy = 5.0;
    int Qvz = 200.0;
};

}
#endif
