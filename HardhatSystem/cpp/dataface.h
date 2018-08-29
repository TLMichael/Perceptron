#ifndef DATAFACE_H
#define DATAFACE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QDateTime>

#include "yolo_v2_class.hpp"

class DataFace
{
public:
    DataFace();
    ~DataFace();
    bool open();
    bool insertVideo(QString path, float fps, int frameCount);
    bool insertDetection(QString path, QVariantList qFrameNow, QVariantList qBoxID, \
                         QVariantList qObjID, QVariantList qX, QVariantList qY, \
                         QVariantList qW, QVariantList qH, QVariantList qProb);
    bool deleteVideo(QString path);
    int searchVideo(QList<QString> &pathL, QList<float> &fpsL, QList<int> &frameCountL, QList<QDateTime> &saveTimeL);
    int searchDetection(QString path, QList<int> &frameNowL, QList<int> &boxIDL, QList<int> &objIDL, QList<int> &xL, QList<int> &yL, QList<int> &wL, QList<int> &hL);
    int countNoHat(int frameNow);
    int countTotal(int frameNow);
    std::vector<bbox_t> getResult(int frameNow);
    int getFrameCount();


private:
    //数据库变量
    QString dbName = "hardhat_system";

    QSqlDatabase db;

    QList<int> qFrameNow, qBoxID, qObjID, qX, qY, qW, qH;
    std::vector<bbox_t> *results;

    int frameCount;
};

#endif // DATAFACE_H


void testDataFace();
