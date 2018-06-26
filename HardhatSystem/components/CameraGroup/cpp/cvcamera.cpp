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

int CVCamera::getDevice() const
{
    return device;
}

void CVCamera::setDevice(int device)
{
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

void CVCamera::update()
{
    qDebug() << "Opening camera " << device << ", Size: " << size;

    //Destroy old thread, camera accessor and buffers
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
    camera = new BetterVideoCapture();
    thread = new CameraThread(camera,videoFrame,cvImageBuf,size.width(),size.height());
    connect(thread,SIGNAL(imageReady()), this, SLOT(imageReceived()));

    //Open newly created device
    try{
        if(camera->open(device)){
            camera->setProperty(CV_CAP_PROP_FRAME_WIDTH,size.width());
            camera->setProperty(CV_CAP_PROP_FRAME_HEIGHT,size.height());
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
        if(!videoSurface->present(*videoFrame))
            DPRINT("Could not present QVideoFrame to QAbstractVideoSurface, error: %d",videoSurface->error());

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
























