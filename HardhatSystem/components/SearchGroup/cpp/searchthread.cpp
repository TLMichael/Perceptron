#include "searchthread.h"
#include "utils.h"

SearchTask::SearchTask( BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    this->running = true;
    this->camera = camera;
    this->videoFrame = videoFrame;
    this->cvImageBuf = cvImageBuf;
    this->width = width;
    this->height = height;
}

SearchTask::~SearchTask()
{
    //Leave camera and videoFrame alone, they will be destroyed elsewhere
}

void SearchTask::stop()
{
    running = false;
}

void SearchTask::doWork()
{
    running = true;
    fps = camera->getProperty(CV_CAP_PROP_FPS);
//    qDebug() << "开始工作";
//    qDebug() << "开始frame: " <<  frameNow;
//    qDebug() << "开始frame: " << camera->getProperty(CV_CAP_PROP_POS_AVI_RATIO) * camera->getProperty(CV_CAP_PROP_FRAME_COUNT);
//    qDebug() << "video的帧指针位置：" << camera->getProperty(CV_CAP_PROP_POS_AVI_RATIO);

    if(videoFrame)
        videoFrame->map(QAbstractVideoBuffer::ReadOnly);

    cv::Mat screenImage;
    if(videoFrame)
        screenImage = cv::Mat(height,width,CV_8UC4,videoFrame->bits());

    while(running && videoFrame != NULL && camera != NULL)
    {
        if(!camera->grabFrame())
            break;
        unsigned char* cameraFrame = camera->retrieveFrame();

        //Get camera image into screen frame buffer
        if(videoFrame)
        {

            // Assuming desktop, RGB camera image and RGBA QVideoFrame
            cv::Mat tempMat(height, width, CV_8UC3, cameraFrame);
            QThread::msleep((fps == 20) ? fps*2 : fps);

            frameNow = camera->getProperty(cv::CAP_PROP_POS_FRAMES);
            // frameNow = (frameNow + 1) % (int)(camera->getProperty(CV_CAP_PROP_FRAME_COUNT) + 1);
            // qDebug() << "opencv frame: " << frameNow;

            std::vector<bbox_t> results = df->getResult(frameNow);

            for(size_t i = 0; i < results.size(); i++)
            {
                int classid = results[i].obj_id;
                int x = results[i].x;
                int y = results[i].y;
                if(classid == 1)
                {
                    cv::putText(tempMat, "No Hardhat!", cv::Point(x, y-10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
                    cv::Rect rec(results[i].x, results[i].y, results[i].w, results[i].h);
                    cv::rectangle(tempMat, rec, cv::Scalar(0, 0, 255), 4);
                }
                else
                {
                    cv::Rect rec(results[i].x, results[i].y, results[i].w, results[i].h);
                    cv::rectangle(tempMat, rec, cv::Scalar(0, 250, 0), 4);
                }
            }
            cv::putText(tempMat, getCurrentTime2().toStdString(), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
            cv::cvtColor(tempMat, screenImage, cv::COLOR_RGB2RGBA);

            if(cvImageBuf)
            {
                // Assuming desktop, RGB camera image
                memcpy(cvImageBuf, cameraFrame, height*width*3);
            }


            emit imageReady();
        }
    }

}


SearchThread::SearchThread(BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    task = new SearchTask(camera,videoFrame,cvImageBuf,width,height);
    task->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), task, SLOT(doWork()));
    connect(task, SIGNAL(imageReady()), this, SIGNAL(imageReady()));
}

SearchThread::~SearchThread()
{
    stop();
    delete task;
}

void SearchThread::start()
{
    if(!workerThread.isRunning())
        workerThread.start();
    else
    {
        task->doWork();
    }
}

void SearchThread::stop()
{
    if(task != NULL)
        task->stop();
    workerThread.quit();
    workerThread.wait();
}

