#ifndef KALAMAN
#define KALAMAN
#include"common.h"

namespace Horizon{

#define MAX_TIME_BIAS 1000
class Kalman_Filter 
{
public:

    Kalman_Filter()
    {
        Eigen::MatrixXd P_in = Eigen::MatrixXd(6,6);
        P_in << 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 1.0;
        P_ = P_in;

        // 过程噪声矩阵附初值
        Eigen::MatrixXd Q_in(6,6);
        Q_in << 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 1.0;
        Q_ = Q_in;

        // 测量矩阵附初值
        Eigen::MatrixXd H_in(3,6);
        H_in << 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
        H_ = H_in;

        // 测量噪声矩阵附初值
        Eigen::MatrixXd R_in(3,3);
        R_in << 1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0;
        R_ = R_in;

        VectorXd last_of_time(6,1);
        last_of_time << 0,0,0,0,0,0;
        last_x_ = last_of_time;

        last_ = Point3f(0,0,0);

    }

    /**
     * @brief 卡尔曼滤波的预测过程
     * 
     * @return  
     */
    void predict();

    /**
     * @brief 卡尔曼滤波的测量过程
     * 
     */
    void measure(VectorXd z);

    /**
     * @brief 卡尔曼主程序
     * 
     */
    Point3f KalmanMainRun(Point3f x_now);

    /**
     * @brief 获取当前时间的函数,单位是毫秒
     * 
     */

private:
    VectorXd x_;                // 当前时刻的状态向量
    VectorXd last_x_;           // 上一次的状态向量
    MatrixXd F_;                // 状态转移矩阵
    MatrixXd B_;                // 输入矩阵
    MatrixXd P_;                // 协方差矩阵，主要数据是对角线上的数据，表示相关性
    MatrixXd K_;                // 卡尔慢增益，过程噪声与测量噪声的比值
    MatrixXd Q_;                // 过程噪声矩阵
    MatrixXd R_;                // 测量噪声矩阵
    MatrixXd H_;                // 测量矩阵
    Eigen::VectorXd y;          // 残差

    Point3f last_;
    
    bool flag_ = false;
    chrono::steady_clock::time_point ta;


};

}
#endif
