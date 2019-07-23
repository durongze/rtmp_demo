#生成h264文件
ffmpegd.exe -i output.mp4 output.264

#推流

#1.设置
#URL    rtmp://192.168.137.7:1935/cctvf
#流名称 durongze
#2.开始推流
#3.不报错，控制台可打印数据代表正常。

#ffmpeg 推流
ffmpegd.exe -re -i test.flv -vcodec copy -acodec copy -f flv -y rtmp://192.168.137.7:1935/cctvf/du
