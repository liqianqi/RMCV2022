#include "kalaman.h"

using namespace Horizon;

void Kalman_Filter::predict()
{
    x_ = F_ * last_x_;

    P_ = F_ * P_ * F_.transpose() + Q_;
}

void Kalman_Filter::measure(VectorXd z)
{
    y = z - H_ * x_;						                //获取测量值与 预测测量值之间的差值

    Eigen::MatrixXd S = H_ * P_ * H_.transpose() + R_;		//临时变量
	K_ = P_ * H_.transpose() * S.inverse();	                //获取卡尔曼增益
	x_ = x_ + (K_ * y);										//获得状态最优估计
 
	int size = x_.size();
	Eigen::MatrixXd I = Eigen::MatrixXd::Identity(size, size);
	P_ = (I - K_ * H_) * P_;		                        // 更新协方差矩阵，完成闭环控制

}

Point3f Kalman_Filter::KalmanMainRun(Point3f now_x)
{
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast <chrono::duration < double>>(t2 - ta);
    ta = t2;

    if(!flag_)
    {
        VectorXd x(6,1);
        x << now_x.x,now_x.y,now_x.z,0,0,0;
        last_x_ = x;
        cout << "kalaman init" << endl;

        flag_ = true;
        return now_x;
    }


    float vx = (now_x.x - last_x_[0])/(time_used.count());

    float vy = (now_x.y - last_x_[0])/(time_used.count());

    float vz = (now_x.z - last_x_[0])/(time_used.count());


    VectorXd x(6,1);
    x << now_x.x,now_x.y,now_x.z,vx,vy,vz;
    x_ = x;

    Eigen::MatrixXd F(6,6);
    F <<	1, 0.0, 0.0, time_used.count(), 0.0, 0.0,
            0.0, 1, 0.0, 0.0, time_used.count(), 0.0,
            0.0, 0.0, 1, 0.0, 0.0, time_used.count(),
            0.0, 0.0, 0.0, 1, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 1, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 1;
    F_ = F;
    cout << "   " << x_[0] << "   " << x_[1] << "   " << x_[2] << endl;
    predict();

    VectorXd z_(3,1);
    z_ << now_x.x,now_x.y,now_x.z;

    measure(z_);

    Point3f return_point;
    return_point.x = x_[0];
    return_point.y = x_[1];
    return_point.z = x_[2];

    last_x_ = x_;

    cout << "   " << x_[0] << "   " << x_[1] << "   " << x_[2] << endl;

    //cout << "   " << last_x_[0] << "   " << last_x_[1] << "   " << last_x_[2] << endl;

    cout << "kalaman filter finished" << endl;
    return return_point;

}



