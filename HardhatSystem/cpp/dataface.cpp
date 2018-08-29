#include "dataface.h"
#include <QDebug>

DataFace::DataFace()
{
//    db=QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("127.0.0.1");
//    db.setUserName("root");
//    db.setPassword("taolue");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    open();
}

DataFace::~DataFace()
{
    delete [] results;
}

bool DataFace::open()
{
    if (!db.open())
    {
        qDebug() << "数据库打开失败！";
        return false;
    }

    QSqlQuery tmpQuery;
    if(!tmpQuery.exec("create table if not exists video (videoID integer primary key autoincrement, path varchar(100) unique, fps float(5,2), frameCount int, saveTime timestamp default current_timestamp)"))
    {
        qDebug() << "创建video表失败！" << tmpQuery.lastError().text();
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
    tmpQuery.prepare("insert into video(path,fps,frameCount) values(?,?,?)");
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
        // qDebug() << "get videoID: " << videoID;
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

bool DataFace::deleteVideo(QString path)
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
        // qDebug() << "get videoID: " << videoID;
    }
    else
    {
        qDebug() << "查询video表，path不存在！";
        return false;
    }

    if(!tmpQuery.exec("delete from detection where videoID='" + QString::number(videoID, 10) + "'"))
    {
        qDebug() << "删除detection表的记录失败！";
        return false;
    }

    if(!tmpQuery.exec("delete from video where videoID='" + QString::number(videoID, 10) + "'"))
    {
        qDebug() << "删除video表的记录失败！";
        return false;
    }
    return true;
}

int DataFace::searchVideo(QList<QString> &pathL, QList<float> &fpsL, QList<int> &frameCountL, QList<QDateTime> &saveTimeL)
{
    QSqlQuery tmpQuery(db);


//    if(!tmpQuery.exec("select videoID,path,fps,frameCount,DATE_FORMAT(saveTime, '%Y-%c-%e %H:%i:%s.%f') as saveTime from video"))
    if(!tmpQuery.exec("select * from video"))
    {
        qDebug() << "查询video表失败！" << tmpQuery.lastError().text();
        return -1;
    }

//    int recordNum = tmpQuery.size();
//    if(recordNum <= 0)
//    {
//        qDebug() << "视频记录为空" << tmpQuery.lastQuery();
//        // return recordNum;
//    }
    int recordNum = 0;
    while(tmpQuery.next())
    {
        recordNum++;
        QString path = tmpQuery.value(1).toString();
        float fps = tmpQuery.value(2).toFloat();
        int frameCount = tmpQuery.value(3).toInt();
        QString saveTimeString = tmpQuery.value(4).toString();
        QDateTime saveTime = QDateTime::fromString(saveTimeString, "yyyy-MM-dd hh:mm:ss");

        pathL << path;
        fpsL << fps;
        frameCountL << frameCount;
        saveTimeL << saveTime;
    }
    return recordNum;
}

int DataFace::searchDetection(QString path, QList<int> &frameNowL, QList<int> &boxIDL, QList<int> &objIDL, QList<int> &xL, QList<int> &yL, QList<int> &wL, QList<int> &hL)
{
    QSqlQuery tmpQuery;

    if(!tmpQuery.exec("select videoID,frameCount from video where path='" + path + "'"))
    {
        qDebug() << "查询video表失败！";
        return -1;
    }

    // qDebug() << "查询到记录个数： " << tmpQuery.size();
    int videoID;

    if(tmpQuery.next())
    {
        videoID = tmpQuery.value(0).toInt();
        frameCount = tmpQuery.value(1).toInt();
        qDebug() << "get videoID: " << videoID;
    }
    else
    {
        qDebug() << "查询video表，path不存在！";
        return -1;
    }

    if(!tmpQuery.exec("select * from detection where videoID='" + QString::number(videoID, 10) + "'"))
    {
        qDebug() << "查询detection表失败！";
        return -1;
    }

//    int recordNum = tmpQuery.size();
//    if(recordNum <= 0)
//    {
//        qDebug() << "视频记录为空";
//        results = new std::vector<bbox_t>[frameCount + 1];
//        return recordNum;
//    }
    int recordNum = 0;

    while(tmpQuery.next())
    {
        recordNum++;
        int frameNow = tmpQuery.value(1).toInt();
        int boxID = tmpQuery.value(2).toInt();
        int objID = tmpQuery.value(3).toInt();
        int x = tmpQuery.value(4).toInt();
        int y = tmpQuery.value(5).toInt();
        int w = tmpQuery.value(6).toInt();
        int h = tmpQuery.value(7).toInt();

        frameNowL.append(frameNow);
        boxIDL.append(boxID);
        objIDL.append(objID);
        xL.append(x);
        yL.append(y);
        wL.append(w);
        hL.append(h);
    }

    this->qFrameNow = frameNowL;
    this->qBoxID = boxIDL;
    this->qObjID = objIDL;
    this->qX = xL;
    this->qY = yL;
    this->qW = wL;
    this->qH = hL;

    results = new std::vector<bbox_t>[frameCount + 1];
    for(int i = 0; i < qFrameNow.size(); i++)
    {
        bbox_t tmpBox;
        tmpBox.obj_id = qObjID[i];
        tmpBox.x = qX[i];
        tmpBox.y = qY[i];
        tmpBox.w = qW[i];
        tmpBox.h = qH[i];

        results[qFrameNow[i]].push_back(tmpBox);
        qDebug() << "得到第" << qFrameNow[i] << "帧: " << results[qFrameNow[i]].size();
    }


//    qDebug() << "frameCount: " << frameCount;
//    qDebug() << "results[4].size: " << results[4].size();
    return recordNum;
}

int DataFace::countNoHat(int frameNow)
{
    int count = 0;
    for(size_t i = 0; i < results[frameNow].size(); i++)
    {
        if(results[frameNow][i].obj_id == 1)
            count++;
    }
    return count;
}

int DataFace::countTotal(int frameNow)
{
    return results[frameNow].size();
}

std::vector<bbox_t> DataFace::getResult(int frameNow)
{
    // qDebug() << "[" << frameNow << "] " << results[frameNow].size();
    return results[frameNow];
}

int DataFace::getFrameCount()
{
    return frameCount;
}


void testDataFace()
{
    //test DataFace
    DataFace df;

    df.insertVideo("/home/michael/1.mp4", 30, 500);

    QVariantList qFrameNow, qBoxID, qObjID, qX, qY, qW, qH, qProb;
    qFrameNow << 1 << 1 << 2 << 2 << 2;
    qBoxID << 0 << 1 << 0 << 1 << 2;
    qObjID << 0 << 0 << 0 << 0 << 0;
    qX << 100 << 100 << 100 << 100 << 100;
    qY << 120 << 120 << 120 << 120 << 120;
    qW << 120 << 120 << 120 << 120 << 120;
    qH << 120 << 120 << 120 << 120 << 120;
    qProb << 99.00 << 50.555 << 130.00 << 0.44 << 22.99;
    df.insertDetection("/home/michael/1.mp4", qFrameNow, qBoxID, qObjID, qX, qY, qW, qH, qProb);

    QList<QString> pathL; QList<float> fpsL; QList<int> frameCountL; QList<QDateTime> saveTimeL;
    int videoCount = df.searchVideo(pathL, fpsL, frameCountL, saveTimeL);
    qDebug() << "videoCount: " << videoCount;
    if(videoCount > 0)
    {
        QString path = pathL.last();
        qDebug() << "path: " << path << ' ' << fpsL.last() << ' ' << frameCountL.last() << ' ' << saveTimeL.last().toString();
        QList<int> frameNowL; QList<int> boxIDL; QList<int> objIDL; QList<int> xL; QList<int> yL; QList<int> wL; QList<int> hL;
        int detectionCount = df.searchDetection(path, frameNowL, boxIDL, objIDL, xL, yL, wL, hL);
        qDebug() << "detectionCount: " << detectionCount;
        for(int i = 0; i < detectionCount; i++)
        {
            qDebug() << "[" << i << "]  " <<  frameNowL.first() << ' ' << boxIDL.first() << ' ' << objIDL.first();
            frameNowL.pop_front();
            boxIDL.pop_front();
            objIDL.pop_front();
        }
    }

}
