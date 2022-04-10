#include "kalaman.h"

namespace Horizon{

void Kalman_Filter::Prediction()
{
    _x = _f * _x;	//+ U_;
    //std::cout << "_x: " << _x << std::endl << std::endl;
    //std::cout << "_f: " << _f << std::endl << std::endl;
    _p = _f * _p * _f.transpose() + _q;					//这一时刻状态协方差矩阵由 上一时刻的自己变换而来，再加上噪声
}

void Kalman_Filter::MeasurementUpdate(const Eigen::VectorXd& z)
{
    Eigen::VectorXd y = z - _h * _x;						//获取测量值与 预测测量值之间的差值
    Eigen::MatrixXd S = _h * _p * _h.transpose() + _r;		//临时变量
    Eigen::MatrixXd K = _p * _h.transpose() * S.inverse();	//获取卡尔曼增益
    _x = _x + (K * y);										//获得状态最优估计
    //std::cout << "y: " << y << std::endl << std::endl;
    int size = _x.size();
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(size, size);
    _p = (I - K * _h) * _p;									//更新状态协方差矩阵
}

<<<<<<< HEAD
cv::Point3f Kalman_Filter::KFilter(cv::Point3f &data)
{
    // 如果是初始化，处于卡尔曼滤波重置的状态
    if(isInitialized())
    {
        // _x << data.x,data.y,data.z,0,0,0;
        _lastOriginData = data;
        return _lastOriginData;
    }
    //float deltaTime_s = _deltaTime_ms;// /1000;    // 目前是毫秒
    //float x = data.x, y = data.y, z = data.z;

    float vx = (data.x - _lastOriginData.x)/_deltaTime_ms;
    float vy = (data.y - _lastOriginData.y)/_deltaTime_ms;
    float vz = (data.z - _lastOriginData.z)/_deltaTime_ms;

    _x << _lastOriginData.x, _lastOriginData.y, _lastOriginData.z, vx, vy, vz;

    // 初始化状态转移矩阵
    _f <<	1.0, 0.0, 0.0, _deltaTime_ms, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0, _deltaTime_ms, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0, _deltaTime_ms,
            0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 1.0;

//    P参数应该由系统自己校正，而不是每一帧赋初值，一旦调好，这个函数应该被注释掉，减小运行时间
//    changeParamQ_R();
//    Prediction();

    Eigen::VectorXd h(3, 1);	//本次观测值
    h << data.x,data.y,data.z;
    MeasurementUpdate(h);

=======
cv::Point3f Kalman_Filter::KFilter(const cv::Point3f &data)
{
    float deltaTime_s = _deltaTime_ms / 1000.0f;
    //float x = data.x, y = data.y, z = data.z;

    float vx = (data.x - _lastOriginData.x)/deltaTime_s;
    float vy = (data.y - _lastOriginData.y)/deltaTime_s;
    float vz = (data.z - _lastOriginData.z)/deltaTime_s;

    float x1 = _lastOriginData.x,y1 = _lastOriginData.y, z1 = _lastOriginData.z;
    Eigen::VectorXd inputX(6, 1);
    inputX << data.x, data.y, data.z, 0, 0, 0;

    Eigen::MatrixXd inputF(6, 6);		//初始化状态转移矩阵

    inputF <<	1.0, 0.0, 0.0, deltaTime_s, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0, deltaTime_s, 0.0,
                0.0, 0.0, 1.0, 0.0, 0.0, deltaTime_s,
                0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 1.0;

    SetF(inputF);

    if (isInitialized()){
        Initialization(inputX);
        _initFlag = false;
        return Point3f(data.x,data.y,data.z);
    }else{
        Eigen::VectorXd lastoriginX(6, 1);
        lastoriginX << x1,y1,z1,vx,vy,vz;
        InitializationX(lastoriginX);
    }
    // 参数应该由系统自己校正，而不是每一帧赋初值，一旦调好，这个函数应该被注释掉，减小运行时间
    changeParamQ_R();
    Prediction();
    Eigen::VectorXd h(3, 1);	//本次观测值
    h << data.x, data.y, data.z;
    MeasurementUpdate(h);



>>>>>>> 这一版增加了数字识别，但效果不太理想
    return Point3f(_x[0],_x[1],_x[2]);
}

cv::Point3f Kalman_Filter::KPredictionRun(cv::Point3f &originData)
{
<<<<<<< HEAD


=======
>>>>>>> 这一版增加了数字识别，但效果不太理想
    _nowTime_ms = cv::getTickCount() / cv::getTickFrequency() * 1000;	//ms
    _deltaTime_ms = (float)(_nowTime_ms - _lastTime_ms);				//ms
    _lastTime_ms = _nowTime_ms;

<<<<<<< HEAD
    //static cv::Point3f lastDataResult = originData;

    //当两次滤波时间间隔大于MAX_TIME_BIASms,视为两次跟随，加一个切帧模式
=======
    // static cv::Point3f lastDataResult = originData;

    // 当两次滤波时间间隔大于MAX_TIME_BIASms,视为两次跟随，加一个切帧模式
>>>>>>> 这一版增加了数字识别，但效果不太理想
    if (_deltaTime_ms > MAX_TIME_BIAS) {
        _lastOriginData = originData;
        _initFlag = true;
        return originData;  // 返回当前数据
    }
    else {
        // 数据跳变处理的最大值和跳跃阈值需要慢慢确定。
<<<<<<< HEAD
        originData = wrongDataKiller(originData);
=======
        //originData = wrongDataKiller(originData);
>>>>>>> 这一版增加了数字识别，但效果不太理想
        // 用来装载卡尔曼滤波后的数据
        originData = KFilter(originData);

        if (isnan(originData.x) || isnan(originData.y) || isnan(originData.z)) {

            cout << "filter data is nan! " << endl;
            originData = _lastOriginData;
            _initFlag = true;		//数据异常，卡尔曼滤波重置
        }
        else {
            _initFlag = false;
        }

        return originData;	//返回当前数据
    }
<<<<<<< HEAD
=======
    return originData;	//返回当前数据
>>>>>>> 这一版增加了数字识别，但效果不太理想
}

cv::Point3f Kalman_Filter::wrongDataKiller(cv::Point3f &originData)
{
    //存在非法坐标
    if (isnan(originData.x) || isnan(originData.y) || isnan(originData.z))
    {
        cout << "Filter originData.x is nan! " << endl;
        return _lastOriginData;
    }

    //剔除不正常坐标
    if (fabs(originData.x) >= _maxLimit.x || fabs(originData.y) >= _maxLimit.y  || fabs(originData.z) >= _maxLimit.z )
    {
        cout << "Filter originData.x is abnormal! " << endl;
        return _lastOriginData;
    }

    cv::Point3f result;
    //剔除跳变
    bool badFlagC = false;		//坐标突变标志位
    //限幅滤波
    //五次以内筛除跳变数据
    if (_badCntC < 5)
    {
        if (fabs(originData.x - _lastOriginData.x) > _jumpLimit.x)
        {
            badFlagC = true;
            result.x = _lastOriginData.x;
        }
        else
        {
            result.x = originData.x;
        }

        if (fabs(originData.y - _lastOriginData.y) > _jumpLimit.y)
        {
            badFlagC = true;
            result.y = _lastOriginData.y;
        }
        else
        {
            result.y = originData.y;
        }

        if (fabs(originData.z - _lastOriginData.z) > _jumpLimit.z)
        {
            badFlagC = true;
            result.z = _lastOriginData.z;
        }
        else
        {
            result.z = originData.z;
        }

    }
    //连续多次出现跳变数据，则相信该数据
    else
    {
        _badCntC = 0;
        result = originData;
        badFlagC = false;
        _initFlag = true;

    }
    //是否出现跳变数据
    if (badFlagC == true)
    {
        _badCntC++;
    }
    else
    {
        _badCntC = 0;
    }

    return result;
}

}
