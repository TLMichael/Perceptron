#include "utils.h"
#include <QDesktopServices>
#include <QDir>
#include <QDebug>

//Detector *detector = new Detector("yolov3.cfg", "yolov3.weights");
Detector *detector = new Detector("yolov3-headhat.cfg", "yolov3-headhat_final.weights");

DataFace *df = new DataFace();

cv::VideoWriter *videoWriter = new cv::VideoWriter();

QString getCurrentTime()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    return time.toString("yyyy-MM-dd-hh-mm-ss"); //设置显示格式
}

QString getCurrentTime2()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    return time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
}

QString getVideoDir()
{
    QString videoDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    videoDir += "/hardhat_video_data/";
    QDir dir(videoDir);
    if(!dir.exists())
        dir.mkdir(videoDir);//只创建一级子目录，即必须保证上级目录存在

    // qDebug() << "videoDir: " << videoDir;
    return videoDir;
}
