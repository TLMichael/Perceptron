#include "utils.h"
#include <QDesktopServices>
#include <QDir>
#include <QDebug>

//Detector *detector = new Detector("yolov3.cfg", "yolov3.weights");
Detector *detector = new Detector("yolov3.cfg", "yolov3.weights");

DataFace *df = new DataFace();

cv::VideoWriter *videoWriter = new cv::VideoWriter();

QSound *sound = new QSound("beep.wav");

QString getCurrentTime()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toString("yyyy-MM-dd-hh-mm-ss");
}

QString getCurrentTime2()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toString("yyyy-MM-dd hh:mm:ss");
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
