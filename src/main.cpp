#include <stdio.h>
#include <string.h>
#include <functional>
#include <thread>
#include "Rmvision.h"

int main(){
	
	Rmvision vision;
	
    thread thread1(&Rmvision::Fronted,ref(vision));
	
    thread thread2(&Rmvision::Backend,ref(vision));

<<<<<<< HEAD
    thread thread3(&Rmvision::Sent_Receive,ref(vision));
=======
    //thread thread3(&Rmvision::Sent_Receive,ref(vision));
>>>>>>> 这一版增加了数字识别，但效果不太理想

    thread1.join();

    thread2.join();

<<<<<<< HEAD
    thread3.join();
=======
    //thread3.join();
>>>>>>> 这一版增加了数字识别，但效果不太理想

	return 0;

}
/**
* ！！！在调之前改文件路径！！！
* 时刻注意时间延迟的问题，只要枪管不抖，定点可以中，就是时间问题
* 调试步骤：
*    1 单双目相机标定，标好参数。
*    2 检查测距是否准确，若不准确，重复第一步操作。
*    3 检查串口通讯是否有问题，串口通讯标准是：数据传输准确，无延迟（至少延迟不可见）。
*    4 将相机坐标系转到云台坐标系，查看目标在云台系的坐标是否准确，在4m范围内要严格，出现问题，可以快速定位到平移向量
*    5 上一步无误后，关掉模式判别，查看跟随是否有问题，调试卡尔曼滤波，使枪管不抖动，稳定跟随。
*    6 查看定点射击是否有问题，有问题改抛物线算法
*    7 进行移动预测，查看迭代法是否有问题，有问题可以追加迭代次数和学习率
*    8 移动预测无问题后，打开模式判别，输出相关目标此时的状态和是否是陀螺
*    9 检查时间单位是否正确
*    10检查枪口指向是否是小陀螺中心
*    11观察陀螺频率和是否开火频率是否相同，大致观察即可
*    12实测小陀螺是否可以按周期打中
*    13极限，犹可突破；至臻，亦不可止！
*/


