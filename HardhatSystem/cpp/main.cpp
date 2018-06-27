// Qt lib import
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
//#include <QVersionNumber>
#include <QMessageBox>
#include <QVariant>

// JQToolsLibrary import
#include <JQToolsLibrary>

// Project lib import
#include <JQToolsManage>

// Group import
#include <WelcomeGroup>
#include <VideoGroup>
#include <CameraGroup>

#include "dataface.h"

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

int main(int argc, char *argv[])
{
    // testDataFace();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    JQToolsManage JQToolsManage;

    JQToolsManage.setQmlApplicationEngine(&engine);


    // Group initialize
    WELCOMEGROUP_INITIALIZA;
    VIDEOGROUP_INITIALIZA;
    CAMERAGROUP_INITIALIZA;

    engine.rootContext()->setContextProperty("JQToolsManage", &JQToolsManage);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
