#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include<QDebug>
#include<QThread>
#include<QObject>
#include<QElapsedTimer>
#include<QVideoFrame>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/videoio/videoio_c.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>

#include <vector>

#include "bettervideocapture.hpp"
#include "yolo_v2_class.hpp"

#define DPRINT(...) while(0);

class SearchTask : public QObject
{
    Q_OBJECT

public:
    SearchTask(BetterVideoCapture* camera, QVideoFrame* videoFrame, unsigned char* cvImageBuf, int width, int height);
    virtual ~SearchTask();
    void stop();

private:
    int width;                                  ///< Width of the camera image
    int height;                                 ///< Height of the camera image
    BetterVideoCapture* camera;                 ///< The camera to get data from
    bool running = false;                       ///< Whether the worker thread is running
    QVideoFrame* videoFrame;                    ///< Place to draw camera image to
    unsigned char* cvImageBuf;                  ///< Place to export camera image to

    int frameNow = 0;
    int fps;

public slots:
    void doWork();

signals:
    void imageReady();

};


class SearchThread : public QObject
{
    Q_OBJECT
public:
    SearchThread(BetterVideoCapture* camera, QVideoFrame* videoFrame, unsigned char* cvImageBuf, int width, int height);
    virtual ~SearchThread();
    void start();
    void stop();

private:

    QThread workerThread;               ///< The thread that the camera will work in
    SearchTask* task = NULL;            ///< The camera loop method and parameter container

signals:
    void imageReady();

};


#endif  //SEARCHTHREAD_H
