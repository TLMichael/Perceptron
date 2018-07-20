#include "camerathread.h"
#include "utils.h"



CameraTask::CameraTask(std::vector<bbox_t> *results, BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    this->running = true;
    this->camera = camera;
    this->videoFrame = videoFrame;
    this->cvImageBuf = cvImageBuf;
    this->width = width;
    this->height = height;
    this->results = results;

}

CameraTask::~CameraTask()
{
    //Leave camera and videoFrame alone, they will be destroyed elsewhere
}

void CameraTask::stop()
{
    imageReady();
    running = false;
}

void CameraTask::doWork()
{
    running = true;
    if(videoFrame)
        videoFrame->map(QAbstractVideoBuffer::ReadOnly);

    cv::Mat screenImage;
    if(videoFrame)
        screenImage = cv::Mat(height,width,CV_8UC4,videoFrame->bits());

    while(running && videoFrame != NULL && camera != NULL)
    {
        if(!camera->grabFrame())
            continue;
        unsigned char* cameraFrame = camera->retrieveFrame();

        //Get camera image into screen frame buffer
        if(videoFrame)
        {
            // Assuming desktop, RGB camera image and RGBA QVideoFrame
            cv::Mat tempMat(height, width, CV_8UC3, cameraFrame);

            std::shared_ptr<image_t> img = detector->mat_to_image(tempMat);
            image_t *iii = img.get();
            std::vector<bbox_t> tmpres = detector->detect(*iii);
            results->assign(tmpres.begin(), tmpres.end());

            qDebug() << "Person numbers: " << results->size();
            cv::putText(tempMat, getCurrentTime2().toStdString(), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
            videoWriter->write(tempMat);

            int nohat = 0;
            for(size_t i = 0; i < results->size(); i++)
            {
                int classid = (*results)[i].obj_id;
                int x = (*results)[i].x;
                int y = (*results)[i].y;
                if(classid == 1)
                {
                    nohat++;
                    cv::putText(tempMat, "No Hardhat!", cv::Point(x, y-10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
                    cv::Rect rec((*results)[i].x, (*results)[i].y, (*results)[i].w, (*results)[i].h);
                    cv::rectangle(tempMat, rec, cv::Scalar(0, 0, 255), 4);
                }
                else
                {
                    cv::Rect rec((*results)[i].x, (*results)[i].y, (*results)[i].w, (*results)[i].h);
                    cv::rectangle(tempMat, rec, cv::Scalar(0, 250, 0), 4);
                }
            }
            // cv::putText(tempMat, "[Total] " + QString::number(results->size(), 10).toStdString() + "   [No Hardhat] " +
            //             QString::number(nohat, 10).toStdString(), cv::Point(10, tempMat.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);


            cv::cvtColor(tempMat, screenImage, cv::COLOR_RGB2RGBA);
        }

        if(cvImageBuf)
        {
            // Assuming desktop, RGB camera image
            memcpy(cvImageBuf, cameraFrame, height*width*3);
        }

        emit imageReady();
    }

}


CameraThread::CameraThread(std::vector<bbox_t> *results, BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    task = new CameraTask(results, camera,videoFrame,cvImageBuf,width,height);
    task->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), task, SLOT(doWork()));
    connect(task, SIGNAL(imageReady()), this, SIGNAL(imageReady()));
}

CameraThread::~CameraThread()
{
    stop();
    delete task;
}

void CameraThread::start()
{
    if(!workerThread.isRunning())
        workerThread.start();
    else
    {
        task->doWork();
    }
}

void CameraThread::stop()
{
    if(task != NULL)
        task->stop();
    workerThread.quit();
    workerThread.wait();
}














