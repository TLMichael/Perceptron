#include "cvcamera.h"

CVCamera::CVCamera(QQuickItem *parent) :
    QQuickItem(parent)
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for(const auto& cameraInfo : cameras)
    {
        QString device;
        device += cameraInfo.deviceName();
        device += " - ";
        device += cameraInfo.description();
        deviceList << device;
    }
    emit deviceListChanged();

    size = QSize(640,480);
    connect(this, &QQuickItem::parentChanged, this, &CVCamera::changeParent);

    //Open camera right away
    //update();
}

CVCamera::~CVCamera()
{
    if(thread)
        thread->stop();
    if(isrunning == true)
        saveDatabase();
    delete thread;
    delete camera;
    //Camera release is automatic when cv::VideoCapture is destroyed

}

void CVCamera::changeParent(QQuickItem* parent)
{
    //FIXME: we probably need to disconnect the previous parent
    //TODO: probably a good idea to stop the camera (and restart it if we are auto-starting in this context)
}

void CVCamera::videoControl()
{
    if(deviceList.size() == 0)
        return;
    if(isrunning == true)
    {
        isrunning = false;
        thread->stop();
        saveDatabase();
    }
    else
    {
        fileUrl = getVideoDir() + getCurrentTime() + ".avi";
        videoWriter->open(fileUrl.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(size.width(), size.height()), true);
        isrunning = true;
        thread->start();
    }
}

int CVCamera::getDevice() const
{
    return device;
}

void CVCamera::setDevice(int device)
{
    qDebug() << "Device: " << device;
    if(device >= 0 && this->device != device){
        this->device = device;
        update();
    }
}

QSize CVCamera::getSize() const
{
    return size;
}

void CVCamera::setSize(QSize size)
{
    if(this->size.width() != size.width() || this->size.height() != size.height()){
        this->size = size;
        update();
        emit sizeChanged();
    }
}

cv::Mat CVCamera::getCvImage()
{
    if(!exportCvImage){
        exportCvImage = true;
        update();
    }
    return cvImage;
}

int CVCamera::getFrameNow() const
{
    return frameNow;
}

int CVCamera::getTotalNow() const
{
    return totalNow;
}

int CVCamera::getNoHatNow() const
{
    return noHatNow;
}

void CVCamera::setFrameNow(int f)
{
    frameNow = f;
}

QStringList CVCamera::getDeviceList() const
{
    return deviceList;
}

void CVCamera::allocateCvImage()
{
    // delete[] cvImageBuf;
    cvImageBuf = new unsigned char[size.width() * size.height() * 3];
    cvImage = cv::Mat(size.height(), size.width(), CV_8UC3, cvImageBuf);
}

void CVCamera::allocateVideoFrame()
{
    videoFrame = new QVideoFrame(size.width() * size.height() * 4, size, size.width() * 4, VIDEO_OUTPUT_FORMAT);
}

void CVCamera::clearQVariant()
{
    qFrameNow.clear();
    qBoxID.clear();
    qObjID.clear();
    qX.clear();
    qY.clear();
    qW.clear();
    qH.clear();
    qProb.clear();
}

bool CVCamera::saveDatabase()
{
    if(deviceList.size() == 0)
        return false;
    qDebug() << "Save video(mp4) to dir: " << fileUrl;
    videoWriter->release();

    qDebug() << "Save detection results to database, size: " << qFrameNow.size();
    df->insertVideo(fileUrl, fps, frameNow);
    bool res = df->insertDetection(fileUrl, qFrameNow, qBoxID, qObjID, qX, qY, qW, qH, qProb);
    qDebug() << "Save detection status: " << res;

    frameNow = 0;
    qFrameNow.clear();
    qBoxID.clear();
    qObjID.clear();
    qX.clear();
    qY.clear();
    qW.clear();
    qH.clear();
    qProb.clear();
    qDebug() << "After save detection results, QVariantList size: " << qFrameNow.size();

    return true;
}

void CVCamera::update()
{
    if(deviceList.size() == 0 || device < 0)
        return;

    BetterVideoCapture precap;
    precap.open(device);
    int w = precap.getProperty(CV_CAP_PROP_FRAME_WIDTH);
    int h = precap.getProperty(CV_CAP_PROP_FRAME_HEIGHT);
    // precap.~BetterVideoCapture();
    qDebug() << "w: " << w << " h: " << h;
    size = QSize(w, h);
    fps = precap.getProperty(CV_CAP_PROP_FPS) - 10;
    qDebug() << "camera fps: " << fps;
    frameNow = 0;
    precap.close();

    qDebug() << "Opening camera " << device << ", Size: " << size;

    //Destroy old thread, camera accessor and buffers
    clearQVariant();
    delete thread;
    delete camera;
    if(videoFrame && videoFrame->isMapped())
        videoFrame->unmap();
    delete videoFrame;
    videoFrame = NULL;
    delete[] cvImageBuf;
    cvImageBuf = NULL;

    //Create new buffers, camera accessor and thread
    if(exportCvImage)
        allocateCvImage();
    if(videoSurface)
        allocateVideoFrame();

    fileUrl = getVideoDir() + getCurrentTime() + ".avi";
    videoWriter->open(fileUrl.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(size.width(), size.height()), true);
    camera = new BetterVideoCapture();
    thread = new CameraThread(&results, camera,videoFrame,cvImageBuf,size.width(),size.height());
    connect(thread,SIGNAL(imageReady()), this, SLOT(imageReceived()));

    //Open newly created device
    try{
        if(camera->open(device)){
            //            camera->setProperty(CV_CAP_PROP_FRAME_WIDTH,size.width());
            //            camera->setProperty(CV_CAP_PROP_FRAME_HEIGHT,size.height());
            if(videoSurface){
                if(videoSurface->isActive())
                    videoSurface->stop();
                if(!videoSurface->start(QVideoSurfaceFormat(size, VIDEO_OUTPUT_FORMAT)))
                    DPRINT("Could not start QAbstractVideoSurface, error: %d",videoSurface->error());
            }
            thread->start();
            qDebug() << "Opened camera " << device;
        }
        else
            qDebug() << "Could not open camera " << device;
    }
    catch(int e){
        qDebug("Exception %d",e);
    }

}

void CVCamera::imageReceived()
{
    //Update VideoOutput
    if(videoSurface)
    {
        if(!videoSurface->present(*videoFrame))
            DPRINT("Could not present QVideoFrame to QAbstractVideoSurface, error: %d",videoSurface->error());

        frameNow++;

        for(size_t i = 0; i < results.size(); i++)
        {
            qFrameNow << frameNow;
            qBoxID << (int)i;
            qObjID << results[i].obj_id;
            qX << results[i].x;
            qY << results[i].y;
            qW << results[i].w;
            qH << results[i].h;
            qProb << results[i].prob;
        }


        noHatNow = 0;
        for(size_t i = 0; i < results.size(); i++)
        {
            int classid = results[i].obj_id;
            if(classid == 1)
                noHatNow++;
        }
        totalNow = results.size();

        emit frameNowChanged();

    }

    //Update exported CV image
    if(exportCvImage)
        emit cvImageChanged();
}

QAbstractVideoSurface* CVCamera::getVideoSurface() const
{
    return videoSurface;
}

void CVCamera::setVideoSurface(QAbstractVideoSurface* surface)
{
    if(videoSurface != surface){
        videoSurface = surface;
        update();
    }
}
























