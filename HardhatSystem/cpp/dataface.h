#ifndef DATAFACE_H
#define DATAFACE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QList>
#include <QDateTime>

class DataFace
{
public:
    DataFace();
    bool open();
    bool insertVideo(QString path, float fps, int frameCount);
    bool insertDetection(QString path, QVariantList qFrameNow, QVariantList qBoxID, \
                         QVariantList qObjID, QVariantList qX, QVariantList qY, \
                         QVariantList qW, QVariantList qH, QVariantList qProb);
    int searchVideo(QList<QString> &pathL, QList<float> &fpsL, QList<int> &frameCountL, QList<QDateTime> &saveTimeL);
    int searchDetection(QString path, QList<int> &frameNowL, QList<int> &boxIDL, QList<int> &objIDL, QList<int> &xL, QList<int> &yL, QList<int> &wL, QList<int> &hL);

private:
    //数据库变量
    QString dbName = "hardhat_system";

    QSqlDatabase db;
};

#endif // DATAFACE_H


void testDataFace();
