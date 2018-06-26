#ifndef DATAFACE_H
#define DATAFACE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

class DataFace
{
public:
    DataFace();
    bool open();
    bool insertVideo(QString path, float fps, int frameCount);
    bool insertDetection(QString path, QVariantList qFrameNow, QVariantList qBoxID, \
                         QVariantList qObjID, QVariantList qX, QVariantList qY, \
                         QVariantList qW, QVariantList qH, QVariantList qProb);

private:
    //数据库变量
    QString dbName = "hardhat_system";

    QSqlDatabase db;
};

#endif // DATAFACE_H
