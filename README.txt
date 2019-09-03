#生成h264文件
ffmpegd.exe -i output.mp4 output.264

#推流

#1.设置
#URL    rtmp://192.168.137.7:1935/cctvf
#流名称 durongze
#2.开始推流
#3.不报错，控制台可打印数据代表正常。

#ffmpeg 推流
ffmpegd.exe -re -i test.flv -vcodec copy -acodec copy -f flv -y rtmp://192.168.137.16:1935/cctvf/du

#!/bin/bash


cd && unzip nginx-rtmp-module-master.zip
tar xf nginx-1.17.1.tar.gz
cd nginx-1.17.1
./configure --prefix=/opt/nginx-1_17_1  --add-module=/opt/nginx-rtmp-module-master --with-http_ssl_module

sudo vi /opt/nginx-1_17_1/conf/nginx.conf
#增加如下内容
#rtmp {
#    server {
#        listen 1935; #监听的端口
#        chunk_size 4000;
#        application cctvf { #rtmp推流请求路径 (切记路径错了会推不上流)
#            live on; #开启实时
#            hls on; #开启hls
#            hls_path /opt/nginx-1_17_1/html/cctvf; #rtmp推流请求路径，文件存放路径
#            hls_fragment 5s; #每个TS文件包含5秒的视频内容
#        }
#    }
#}

#启动服务器
/opt/nginx-1_17_1/sbin/nginx -c /opt/nginx-1_17_1/conf/nginx.conf

#推流 
#1.设置
#URL    rtmp://192.168.137.7:1935/cctvf
#流名称 durongze
#2.开始推流
#3.验证 可以在hls_path 中看到视频文件,停止推流后文件会自动删除
#ffmpeg 推流
ffmpegd.exe -re -i test.flv -vcodec copy -acodec copy -f flv -y rtmp://192.168.137.7:1935/cctvf/du
####ffmpegd -list_devices true -f dshow -i dummy  查看摄像头名称
ffmpeg -f dshow -i video="Integrated Webcam"  -acodec copy -preset:v ultrafast  -f flv rtmp://192.168.137.157:1935/cctvf/du
ffmpeg -f gdigrab -i desktop  -preset:v ultrafast  -f flv  rtmp://192.168.137.157:1935/cctvf/du
#拉流 
#1.打开串口流  注意nginx服务器端口默认为80 
#http://192.168.137.7/cctvf/durongze.m3u8
#ffplay 拉流
ffplayd rtmp://192.168.137.157:1935/cctvf/du
#生成h264文件
ffmpegd.exe -i output.mp4 output.264