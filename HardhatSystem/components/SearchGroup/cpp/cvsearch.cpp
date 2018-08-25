#include "cvsearch.h"

CVSearch::CVSearch(QQuickItem *parent) :
    QQuickItem(parent)
{
    size = QSize(640,480);
    connect(this, &QQuickItem::parentChanged, this, &CVSearch::changeParent);
    connect(this, SIGNAL(fileUrlChanged()), this, SLOT(fileUrlGeted()));
}

CVSearch::~CVSearch()
{
    if(thread)
        thread->stop();
    delete thread;
    delete video;
    //Video release is automatic when cv::VideoCapture is destroyed
}

void CVSearch::changeParent(QQuickItem* parent)
{
    //FIXME: we probably need to disconnect the previous parent
    //TODO: probably a good idea to stop the camera (and restart it if we are auto-starting in this context)
}

void CVSearch::videoControl()
{
    //    qDebug() << "frameCount: " << frameCount;
    //    qDebug() << "frameNow: " << frameNow;
    if(frameCount == frameNow && frameCount > 0)
    {
        frameNow = 0;
        thread->stop();

        // video->setProperty(CV_CAP_PROP_POS_AVI_RATIO, 0);    // This code doesn't work, so weird
        video->open(fileUrl);

        clearQVariant();
        isrunning = true;
        thread->start();
        return;
    }

    if(isrunning == true)
    {
        isrunning = false;
        thread->stop();
    }
    else
    {
        isrunning = true;
        thread->start();
    }
}

int CVSearch::getFrameCount()
{
    return frameCount;
}

void CVSearch::fileUrlGeted()
{
    if(fileUrl == "0")
    {
        qDebug() << "收到空文件路径";
        clearQVariant();
        delete thread;
        delete video;
        if(videoFrame && videoFrame->isMapped())
            videoFrame->unmap();
        delete videoFrame;
        videoFrame = NULL;
        delete[] cvImageBuf;
        cvImageBuf = NULL;

        return;
    }
    // Open video now
    qDebug() << "signal getUrl: " << fileUrl << endl;
    detectionCount = df->searchDetection(fileUrl, qFrameNow, qBoxID, qObjID, qX, qY, qW, qH);
    update();
}


QString CVSearch::getFileUrl() const
{
    return fileUrl;
}

void CVSearch::setFileUrl(QString fileUrl)
{
    if(fileUrl != NULL && this->fileUrl != fileUrl)
    {
        this->fileUrl = fileUrl;
        emit fileUrlChanged();
    }
}

QSize CVSearch::getSize() const
{
    return size;
}

void CVSearch::setSize(QSize size)
{
    if(this->size.width() != size.width() || this->size.height() != size.height()){
        this->size = size;
        update();
        emit sizeChanged();
    }
}

cv::Mat CVSearch::getCvImage()
{
    if(!exportCvImage){
        exportCvImage = true;
        update();
    }
    return cvImage;
}

QString CVSearch::getHeatMap()
{
    return heatMapString;
}

int CVSearch::getFrameNow() const
{
    return frameNow;
}

int CVSearch::getTotalNow() const
{
    return totalNow;
}

int CVSearch::getNoHatNow() const
{
    return noHatNow;
}

void CVSearch::setFrameNow(int f)
{
    frameNow = f - 1;
    update();
}

void CVSearch::updateHeatMap()
{
    // qFrameNow, qBoxID, qObjID
    if(frameCount <= 0 )
        return;

    QVector<int> heatList(frameCount, 0);
    for(int i = 0; i < qFrameNow.size(); i++)
    {
        // qDebug() << qFrameNow[i] << ": " << qBoxID[i] << " " << qObjID[i];
        if(qObjID[i] == 1)
        {
            int idx = qFrameNow[i] - 1;
            heatList[idx]++;
        }
    }

    heatMap = new cv::Mat(frameCount / 10, frameCount, CV_8UC3, cv::Scalar(255, 255, 255));

    for(int i = 0; i < heatList.size(); i++)
    {
        if(heatList[i] > 0)
        {
            cv::Point start(i, 0);
            cv::Point end(i, frameCount / 10);

            int x = heatList[i] > 6 ? 6 : heatList[i];
            int b, g, r;
            r = 255;
            b = g = 140 - 36 * x;
            cv::Scalar color(b, g, r);

            cv::line(*heatMap, start, end, color);
        }
    }

    //cv::blur((*heatMap), blur, cv::Size(5, 5));
    cv::GaussianBlur(*heatMap, *heatMap, cv::Size(5, 5), 0, 0);

    cv::cvtColor((*heatMap), (*heatMap), cv::COLOR_BGR2RGB);
    qHeatMap = QImage((*heatMap).data, (*heatMap).cols, (*heatMap).rows, int((*heatMap).step), QImage::Format_RGB888);

    // QImage to QString
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    qHeatMap.save(&buffer, "JPEG");
    heatMapString = "data:image/jpg;base64,";
    heatMapString.append(QString::fromLatin1(bArray.toBase64().data()));

    emit heatMapChanged();
    //    for(int i = 0; i < heatList.size(); i++)
    //        qDebug() << heatList[i];
}


void CVSearch::allocateCvImage()
{
    // delete[] cvImageBuf;
    cvImageBuf = new unsigned char[size.width() * size.height() * 3];
    cvImage = cv::Mat(size.height(), size.width(), CV_8UC3, cvImageBuf);
}

void CVSearch::allocateVideoFrame()
{
    videoFrame = new QVideoFrame(size.width() * size.height() * 4, size, size.width() * 4, VIDEO_OUTPUT_FORMAT);
}

void CVSearch::clearQVariant()
{
    qFrameNow.clear();
    qBoxID.clear();
    qObjID.clear();
    qX.clear();
    qY.clear();
    qW.clear();
    qH.clear();
}


void CVSearch::update()
{
    BetterVideoCapture precap;
    precap.open(fileUrl);
    int w = precap.getProperty(CV_CAP_PROP_FRAME_WIDTH);
    int h = precap.getProperty(CV_CAP_PROP_FRAME_HEIGHT);
    qDebug() << "w: " << w << " h: " << h;
    size = QSize(w, h);
    frameCount = precap.getProperty(CV_CAP_PROP_FRAME_COUNT);
    fps = precap.getProperty(CV_CAP_PROP_FPS);
    frameNow = 0;

    updateHeatMap();

    qDebug() << "Opening video " << fileUrl << ", Size: " << size;

    //Destroy old thread, camera accessor and buffers
    clearQVariant();
    videoFrame = NULL;
    cvImageBuf = NULL;

    //    qDebug() << "Util now, OK";
    //Create new buffers, camera accessor and thread
    if(exportCvImage)
        allocateCvImage();
    if(videoSurface)
        allocateVideoFrame();

    video = new BetterVideoCapture();

    thread = new SearchThread(video, videoFrame, cvImageBuf, size.width(), size.height());
    connect(thread, SIGNAL(imageReady()), this, SLOT(imageReceived()));

    //Open newly created device
    try{
        if(video->open(fileUrl)){
            // video->setProperty(CV_CAP_PROP_FRAME_WIDTH,size.width());
            // video->setProperty(CV_CAP_PROP_FRAME_HEIGHT,size.height());
            if(videoSurface){
                if(videoSurface->isActive())
                    videoSurface->stop();
                if(!videoSurface->start(QVideoSurfaceFormat(size, VIDEO_OUTPUT_FORMAT)))
                    DPRINT("Could not start QAbstractVideoSurface, error: %d",videoSurface->error());
            }
            thread->start();
            qDebug() << "Opened video " << fileUrl;
        }
        else
            qDebug() << "Could not open video " << fileUrl;
    }
    catch(int e){
        qDebug("Exception %d",e);
    }

}

void CVSearch::imageReceived()
{
    //Update VideoOutput
    if(videoSurface)
    {
        if(!videoSurface->present(*videoFrame))
            DPRINT("Could not present QVideoFrame to QAbstractVideoSurface, error: %d",videoSurface->error());

        frameNow++;
        qDebug() << "frameNow: " << frameNow << "/" << frameCount;
        // QVariantList qFrameNow, qBoxID, qObjID, qX, qY, qW, qH, qProb;

        noHatNow = df->countNoHat(frameNow);
        totalNow = df->countTotal(frameNow);
        emit frameNowChanged();
    }


    //Update exported CV image
    if(exportCvImage)
        emit cvImageChanged();

}

QAbstractVideoSurface* CVSearch::getVideoSurface() const
{
    return videoSurface;
}

void CVSearch::setVideoSurface(QAbstractVideoSurface* surface)
{
    if(videoSurface != surface){
        videoSurface = surface;
        // update();
    }
}
