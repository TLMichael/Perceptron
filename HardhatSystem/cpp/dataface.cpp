#include "dataface.h"
#include <QDebug>

DataFace::DataFace()
{
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("taolue");
    db.setDatabaseName(dbName);
}

bool DataFace::open()
{
    if (!db.open())
    {
        qDebug() << "数据库打开失败！";
        return false;
    }

    QSqlQuery tmpQuery;
    if(!tmpQuery.exec("create table if not exists video (videoID char(10) primary key, path varchar(100), fps float(5,2), frameCount int)"))
    {
        qDebug() << "Video表失败！";
        return false;
    }
//    if(!tmpQuery.exec("create table if not exists detection (v)"))





    return true;
}
