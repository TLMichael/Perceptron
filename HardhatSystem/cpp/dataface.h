#ifndef DATAFACE_H
#define DATAFACE_H

#include <QSqlDatabase>
#include <QSqlQuery>

class DataFace
{
public:
    DataFace();
    bool open();

private:
    //数据库变量
    QString dbName = "hardhat_system";

    QSqlDatabase db;
    QSqlQuery query;
    QSqlQuery *videoQuery;
    QSqlQuery *detectionQuery;
};

#endif // DATAFACE_H
