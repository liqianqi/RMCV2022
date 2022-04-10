﻿#include "Send_Receive.h"

namespace Horizon{
#define DATA_LENGTH 16               //接受的数据位数
#define SERIAL_RECIVER_TRANSFER_TIME  0.001875f

int OpenPort(const char *Portname)
{
    int fd;
//    fd = open(Portname,O_RDWR);
    fd = open(Portname,O_RDWR | O_NOCTTY | O_NONBLOCK);
    cout << fd << endl;
    if(-1 == fd)
    {
        printf("The port open error!\n");
        return -1;
    }
    else
    {
        fcntl(fd,F_SETFL,0);   //读取串口的信息
    }
    return fd;
}

int configureSerial(int fd){
    struct termios port_set;
    //波特率
    cfsetispeed(&port_set,B115200);
    cfsetospeed(&port_set,B115200);
    //No parity
    port_set.c_cflag &= ~PARENB;         //无奇偶校验
    port_set.c_cflag &= ~CSTOPB;         //停止位:1bit
    port_set.c_cflag &= ~CSIZE;          //清除数据位掩码
    port_set.c_cflag |=  CS8;

    tcsetattr(fd,TCSANOW,&port_set);
    return (fd);
}

void DataControler::sentData(int fd,VisionData data){

    unsigned char send_bytes[15] = {0};

    send_bytes[0] = 0xaa;
    if(data.pitch_data_.f < 0){
        SET_BIT((data.OnePointFive),8);
    }else{
        CLEAR_BIT((data.OnePointFive),8);
    }
  //   SET_BIT((data.OnePointFive),1);

    if(data.yaw_data_.f < 0){
        SET_BIT((data.OnePointFive),7);
    }else{
        CLEAR_BIT((data.OnePointFive),7);
    }
    data.yaw_data_.f = fabs(data.yaw_data_.f);
    data.pitch_data_.f = fabs(data.pitch_data_.f);

<<<<<<< HEAD
   // data.yaw_data_.f = 10;
   // data.pitch_data_.f = 10;
=======
>>>>>>> 这一版增加了数字识别，但效果不太理想
   // data.time.f = 10;
    send_bytes[1] = data.pitch_data_.c[0];
    send_bytes[2] = data.pitch_data_.c[1];
    send_bytes[3] = data.pitch_data_.c[2];
    send_bytes[4] = data.pitch_data_.c[3];

    send_bytes[5] = data.yaw_data_.c[0];
    send_bytes[6] = data.yaw_data_.c[1];
    send_bytes[7] = data.yaw_data_.c[2];
    send_bytes[8] = data.yaw_data_.c[3];

<<<<<<< HEAD
//    state_ = 0;
=======
    state_ = 0;
>>>>>>> 这一版增加了数字识别，但效果不太理想
//    SET_BIT((data.OnePointFive),6);
//    CLEAR_BIT((data.OnePointFive),7);
//    CLEAR_BIT((data.OnePointFive),8);
    if(state_ == 0){//zimiao  001

        SET_BIT((data.OnePointFive),1);
        CLEAR_BIT((data.OnePointFive),2);
        CLEAR_BIT((data.OnePointFive),3);

    }else if(state_ == 1){//dafu   110

        SET_BIT((data.OnePointFive),3);
        SET_BIT((data.OnePointFive),2);
        CLEAR_BIT((data.OnePointFive),1);

    }else if(state_ == 2){//xiaofu   100

        SET_BIT((data.OnePointFive),3);
        CLEAR_BIT((data.OnePointFive),2);
        CLEAR_BIT((data.OnePointFive),1);

    }else{//fantuoluo   010

        CLEAR_BIT((data.OnePointFive),3);
        SET_BIT((data.OnePointFive),2);
        CLEAR_BIT((data.OnePointFive),1);

    }

    if(data.is_fire == 1){
        SET_BIT((data.OnePointFive),4);
    }else{
        CLEAR_BIT((data.OnePointFive),4);
    }

    if(data.is_have_armor == 1){
        SET_BIT((data.OnePointFive),5);
    }else{
        CLEAR_BIT((data.OnePointFive),5);
    }


  //   SET_BIT((data.OnePointFive),2);

    send_bytes[9] = data.OnePointFive;

    //data.time.f = 1;
    send_bytes[10] = data.time.c[0];
    send_bytes[11] = data.time.c[1];
    send_bytes[12] = data.time.c[2];
    send_bytes[13] = data.time.c[3];

    send_bytes[14] = 0xbb;
    //cout << "hhhhhhhhhhhh" << endl;
    write(fd, send_bytes, 15);
}

void DataControler::getData(int fd,Stm32Data &get_data){
    int bytes = 0;
    //uint8_t cde;
    //unsigned char rec_bytes[1024] = {0};
    // 这是干什么的
    ioctl(fd, FIONREAD, &bytes);//1199
    if(bytes < DATA_LENGTH) {
        return ;
    }
    unsigned char* rec_bytes = new unsigned char[bytes+10] ();
    bytes = read(fd,rec_bytes,bytes);
    // cout << "<<<<<<<<<<<<<<>>>>>>KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK" << bytes << endl;
    // 112
    // cout << "<<<<<<<<<<<<<<>>>>>>KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK" << endl;
    // 这两个索引是干什么的
    int FirstIndex = -1;
    int LastIndex = -1;

    // 这一部是什么逻辑
    for(int i = 0;i < bytes;i++){

        if(rec_bytes[i] == 0xaa&&FirstIndex == -1){
            FirstIndex = i;
        }else if(rec_bytes[i] == 0xbb&&FirstIndex != -1&&i - FirstIndex == DATA_LENGTH-1){
            LastIndex = i;
            break;
        }
    }

    cout << endl;
    //cout << "hello << " << endl;

    if(FirstIndex != -1 && LastIndex != -1){
        //get_data.IsHave = true;
        get_data.pitch_data_.c[0] = rec_bytes[FirstIndex+1];
        get_data.pitch_data_.c[1] = rec_bytes[FirstIndex+2];
        get_data.pitch_data_.c[2] = rec_bytes[FirstIndex+3];
        get_data.pitch_data_.c[3] = rec_bytes[FirstIndex+4];


        get_data.yaw_data_.c[0] = rec_bytes[FirstIndex+5];
        get_data.yaw_data_.c[1] = rec_bytes[FirstIndex+6];
        get_data.yaw_data_.c[2] = rec_bytes[FirstIndex+7];
        get_data.yaw_data_.c[3] = rec_bytes[FirstIndex+8];

        //cout << "????????????????????LLLLLLKKKKKKKKKKKIIIIIIIIIIIPOOfffff" << "           "<< get_data.yaw_data_.f << endl;

        get_data.OnePointFive = rec_bytes[FirstIndex+9];

        if(getBit(get_data.OnePointFive,1) == 0 && getBit(get_data.OnePointFive,2) == 0 && getBit(get_data.OnePointFive,3) == 1) {
            get_data.flag = 0;
        }else if(getBit(get_data.OnePointFive,1) == 1 && getBit(get_data.OnePointFive,2) == 1 && getBit(get_data.OnePointFive,3) == 0){
            get_data.flag = 1;
        }else if(getBit(get_data.OnePointFive,1) == 1 && getBit(get_data.OnePointFive,2) == 0 && getBit(get_data.OnePointFive,3) == 0){
            get_data.flag = 2;
        }else if(getBit(get_data.OnePointFive,1) == 0 && getBit(get_data.OnePointFive,2) == 1 && getBit(get_data.OnePointFive,3) == 0){
            get_data.flag = 3;
        }

        if(getBit(get_data.OnePointFive,8) == 0){
            get_data.pitch_data_.f = fabs(get_data.pitch_data_.f);
        }else{
            get_data.pitch_data_.f = -fabs(get_data.pitch_data_.f);
        }

        if(getBit(get_data.OnePointFive,7) == 0){
            get_data.yaw_data_.f = fabs(get_data.yaw_data_.f);
        }else{
            get_data.yaw_data_.f = -fabs(get_data.yaw_data_.f);
        }

        get_data.time.c[0] = rec_bytes[FirstIndex+10];
        get_data.time.c[1] = rec_bytes[FirstIndex+11];
        get_data.time.c[2] = rec_bytes[FirstIndex+12];
        get_data.time.c[3] = rec_bytes[FirstIndex+13];

        //设置射速
        get_data.init_firing_rate = rec_bytes[FirstIndex+14];

        //get_data.IsHave = true;
        cout<<"接收完成"<<get_data.time.f<<endl;
        get_data.dubug_print = true;
    }else{
        cout<<"接受失败!"<<endl;
        //get_data.IsHave = false;
        get_data.dubug_print = false;

    }

    //return;

    return ;

}


}
