#include "videothread.h"
#include "utils.h"

VideoTask::VideoTask(BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    this->running = true;
    this->camera = camera;
    this->videoFrame = videoFrame;
    this->cvImageBuf = cvImageBuf;
    this->width = width;
    this->height = height;

}

VideoTask::~VideoTask()
{
    //Leave camera and videoFrame alone, they will be destroyed elsewhere
}

void VideoTask::stop()
{
    running = false;
}

void helloWorld(cv::Mat &img, int x, int y)
{
    cv::RNG rng(time(0));
    cv::putText(img, "FUCK THE WORLD", cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 1.6, cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), 8);

}

void VideoTask::doWork()
{
    running = true;
    if(videoFrame)
        videoFrame->map(QAbstractVideoBuffer::ReadOnly);

    cv::Mat screenImage;
    if(videoFrame)
        screenImage = cv::Mat(height,width,CV_8UC4,videoFrame->bits());

    int x = 80;
    int y = 150;
    cv::RNG rng(time(0));
    QMap< unsigned int, cv::Scalar > colors;
    for(int i=0; i < 80; i++)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        colors.insert(i, color);
    }





    while(running && videoFrame != NULL && camera != NULL)
    {
        QThread::msleep(10);
        if(x < 350)
            x += 7;
        else
            x = 80;
        if(y < 650)
            y += 7;
        else
            y = 150;


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
            std::vector<bbox_t> results;
            results = detector->detect(*iii);

            qDebug() << "Person numbers: " << results.size();
            for(size_t i = 0; i < results.size(); i++)
            {
                qDebug() << "[" << i << "] " << results[i].x << " " << results[i].y << " " << results[i].w << " " << results[i].h;
                cv::Rect rec(results[i].x, results[i].y, results[i].w, results[i].h);

                cv::rectangle(tempMat, rec, colors.find(results[i].obj_id).value(), 4);
            }

            helloWorld(tempMat, x, y);

            // cv::imshow("test", tempMat);



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



VideoThread::VideoThread(BetterVideoCapture *camera, QVideoFrame *videoFrame, unsigned char *cvImageBuf, int width, int height)
{
    task = new VideoTask(camera,videoFrame,cvImageBuf,width,height);
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














