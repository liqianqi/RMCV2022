#pragma once
#include"common.h"
namespace Horizon{
    /**
     * @brief 陀螺自瞄专用一维滤波器
     */
    class KalmanFilter
    {
    public:
        KalmanFilter(){};
        ~KalmanFilter() {}
        void setParam();
        double predict(double);

    private:
        bool    m_initialized{false};
        int     m_debug;
        int     m_measure_num = 1;
        int     m_state_num   = 2;
        int     m_lost_count;
        int     m_init_count_threshold;
        float   m_control_freq;
        float   m_predict_coe;
        // double  m_process_noise;
        // double  m_measure_noise;
        double  m_last_val;
        double  m_val_diff;
        cv::Mat m_measurement;
        std::shared_ptr<cv::KalmanFilter> m_KF;

        void initFilter(double);
    };
}    
