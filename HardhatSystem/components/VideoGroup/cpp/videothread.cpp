#include "videothread.h"
#include "utils.h"

VideoTask::VideoTask(std::vector<bbox_t> *results, BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    this->running = true;
    this->camera = camera;
    this->videoFrame = videoFrame;
    this->cvImageBuf = cvImageBuf;
    this->width = width;
    this->height = height;
    this->results = results;

}

VideoTask::~VideoTask()
{
    //Leave camera and videoFrame alone, they will be destroyed elsewhere
}

void VideoTask::stop()
{
    running = false;
}

void VideoTask::doWork()
{
    running = true;
    if(videoFrame)
        videoFrame->map(QAbstractVideoBuffer::ReadOnly);

    cv::Mat screenImage;
    if(videoFrame)
        screenImage = cv::Mat(height,width,CV_8UC4,videoFrame->bits());

    while(running && videoFrame != NULL && camera != NULL)
    {
        // QThread::msleep(10);


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
            for(size_t i = 0; i < results->size(); i++)
            {
                // qDebug() << "[" << i << "] " << results[i].x << " " << results[i].y << " " << results[i].w << " " << results[i].h;
                cv::Rect rec((*results)[i].x, (*results)[i].y, (*results)[i].w, (*results)[i].h);

                // cv::rectangle(tempMat, rec, colors.find(results[i].obj_id).value(), 4);
                cv::rectangle(tempMat, rec, obj_id_to_color((*results)[i].obj_id), 4);
            }

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



VideoThread::VideoThread(std::vector<bbox_t> *results, BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    task = new VideoTask(results, camera,videoFrame,cvImageBuf,width,height);
    task->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), task, SLOT(doWork()));
    connect(task, SIGNAL(imageReady()), this, SIGNAL(imageReady()));
}

VideoThread::~VideoThread()
{
    stop();
    delete task;
}

void VideoThread::start()
{
    if(!workerThread.isRunning())
        workerThread.start();
    else
    {
        task->doWork();
    }
}

void VideoThread::stop()
{
    if(task != NULL)
        task->stop();
    workerThread.quit();
    workerThread.wait();
}














