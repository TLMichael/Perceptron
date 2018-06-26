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
    if(!tmpQuery.exec("create table if not exists video (videoID int primary key auto_increment, path varchar(100) unique, fps float(5,2), frameCount int, saveTime timestamp default current_timestamp())"))
    {
        qDebug() << "创建video表失败！";
        return false;
    }
    if(!tmpQuery.exec("create table if not exists detection (videoID int, frameNow int, boxID int, objID int, x int, y int, w int, h int, prob float(5,2), foreign key(videoID) references video(videoID) on delete cascade on update cascade, primary key(videoID, frameNow, boxID))"))
    {
        qDebug() << "创建detection表失败！";
        return false;
    }

    return true;
}

bool DataFace::insertVideo(QString path, float fps, int frameCount)
{
    QSqlQuery tmpQuery;

    if(!tmpQuery.exec("select videoID from video where path='" + path + "'"))
    {
        qDebug() << "查询video表失败！";
        return false;
    }
    if(tmpQuery.next())
    {
        qDebug() << "文件path已存在表中，更新数据库";
        if(!tmpQuery.exec("delete from video where path = '" + path + "'"))
        {
            qDebug() << "级联删除video表的path记录和detection表的相应记录失败！";
            return false;
        }
    }

    // 插入新video纪录
    tmpQuery.prepare("insert into video values(NULL,?,?,?,default)");
    QVariantList qpath, qfps, qframecount;
    qpath << path;
    qfps << fps;
    qframecount << frameCount;
    tmpQuery.addBindValue(qpath);
    tmpQuery.addBindValue(qfps);
    tmpQuery.addBindValue(qframecount);

    if(!tmpQuery.execBatch())
    {
        qDebug() << "插入数据失败！";
        qDebug() << tmpQuery.lastError().text();
        return false;
    }
    return true;
}

bool DataFace::insertDetection(QString path, QVariantList qFrameNow, QVariantList qBoxID, QVariantList qObjID, QVariantList qX, QVariantList qY, QVariantList qW, QVariantList qH, QVariantList qProb)
{
    QSqlQuery tmpQuery;

    if(!tmpQuery.exec("select videoID from video where path='" + path + "'"))
    {
        qDebug() << "查询video表失败！";
        return false;
    }

    // qDebug() << "查询到记录个数： " << tmpQuery.size();
    int videoID;
    if(tmpQuery.next())
    {
        videoID = tmpQuery.value(0).toInt();
        qDebug() << "get videoID: " << videoID;
    }
    else
    {
        qDebug() << "查询video表，path不存在！";
        return false;
    }


    // 插入新video的所有detecion
    db.transaction();
    QString query = "insert into detection values(" + QString::number(videoID, 10) + ",?,?,?,?,?,?,?,?)";
    tmpQuery.prepare(query);
    tmpQuery.addBindValue(qFrameNow);
    tmpQuery.addBindValue(qBoxID);
    tmpQuery.addBindValue(qObjID);
    tmpQuery.addBindValue(qX);
    tmpQuery.addBindValue(qY);
    tmpQuery.addBindValue(qW);
    tmpQuery.addBindValue(qH);
    tmpQuery.addBindValue(qProb);

    if(!tmpQuery.execBatch())
    {
        qDebug() << "插入detection表失败！";
    }

    db.commit();

    return true;
}
