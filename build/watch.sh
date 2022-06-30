#! /bin/bash

#Provides:		watchdog					#保存的文件名
#Required-Start:	$remote_fs $syslog
#Required-Stop:		$remote_fs $syslog
#Default-Start:		2 3 4 5
#Default-Stop:		0 1 6
#short-Description:	start watchDog
#Description:		start watchDog
sec=1 
cnt=0
PROC_NAME=RMCV_2022_1_1 					   #要执行的文件，make以后的文件
Thread=`ps -ef | grep $PROC_NAME | grep -v "grep"` 
cd /home/liqianqi/RMCV_2022/build/  		   #文件目录，建议右键点属性查看
make clean && make -j
echo '286740102' | sudo -S sudo chmod +777 /dev/ttyUSB0     # ‘123’ 为ubuntu密码
#echo "286740102" | sudo -S sudo chmod +777 /dev/ttyUSB1     # ttyUSB1 这里只是个示例
./RMCV_2022_1_1 

while [ 1 ] /
do
count=`ps -ef | grep $PROC_NAME | grep -v "grep" | wc -l` 
echo "Thread count: $count"
if [ $count -gt 1 ];then 
	echo "The $PROC_NAME is still alive!"
	sleep $sec 
else  
	echo "Starting $PROC_NAME"
	cd ~
	echo '286740102' | sudo -S sudo chmod +777 /dev/ttyUSB0   	  
	#echo "286740102" | sudo -S sudo chmod +777 /dev/ttyUSB1 
	cd /home/liqianqi/RMCV_2022/build/
	./RMCV_2022_1_1	
	echo "$PROC_NAME has started!"
		sleep $sec
fi  
done
 
