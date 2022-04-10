#include "antigyro_kalaman.h"

namespace Horizon{

    /*
     * @breif 滤波器初始化
     *
     * @param a_val 第一帧的值
     */
    void KalmanFilter::initFilter(double a_val)
    {
        m_KF->statePost = (cv::Mat_<float>(m_state_num, 1) << a_val, 0);
        m_KF->predict();
        m_measurement.at<float>(0) = a_val;

        for(int i = 0; i < m_init_count_threshold; i++)
        {
            m_KF->correct(m_measurement);
            m_KF->predict();
        }
        std::cout << "Init" << std::endl;
    }


    /*
     * @breif 根据新的测量量滤波
     *
     * @param a_val 当前测量量
     *
     * @return 返回滤波后的量
     */
    double KalmanFilter::predict(double a_val)
    {
        cv::Mat result;

        if(!m_initialized)
        {
            initFilter(a_val);
            m_last_val = a_val;
            m_initialized = true;
        }

        m_measurement.at<float>(0) = a_val;
        m_KF->correct(m_measurement);
        result = m_KF->predict();
        if(std::fabs(a_val - m_last_val) > m_val_diff)
        {
            initFilter(a_val);
            if(m_debug)
                std::cout << "Target has changed" << std::endl;
        }
        m_last_val = a_val;

        return 1.0 * result.at<float>(0) + 0.0 * result.at<float>(1);
    }

}
