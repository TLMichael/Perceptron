/**
 * @file cvcamera.h
 * @brief Nice QML wrapper for camera access with OpenCV
 */

#ifndef CVCAMERA_H
#define CVCAMERA_H

#include<QQuickItem>
#include<QCameraInfo>
#include<QList>
#include<QAbstractVideoSurface>
#include<QVideoSurfaceFormat>
#include<QSize>
#include<QMutex>
#include<QWaitCondition>
#include<QVariant>

#include<opencv2/highgui/highgui.hpp>

#include "camerathread.h"
#include "utils.h"

#define CVCAMERA_INITIALIZA                                               \
{                                                                       \
    qmlRegisterType<CVCamera>("com.nuaa.CVCamera", 1, 0, "CVCamera"); \
}



class CVCamera : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(CVCamera)
    Q_PROPERTY(int frameNow READ getFrameNow WRITE setFrameNow NOTIFY frameNowChanged)
    Q_PROPERTY(int totalNow READ getTotalNow NOTIFY totalNowChanged)
    Q_PROPERTY(int noHatNow READ getNoHatNow NOTIFY noHatNowChanged)

    Q_PROPERTY(int device READ getDevice WRITE setDevice)
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getVideoSurface WRITE setVideoSurface)
    Q_PROPERTY(QSize size READ getSize WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QStringList deviceList READ getDeviceList NOTIFY deviceListChanged)
    Q_PROPERTY(cv::Mat cvImage READ getCvImage NOTIFY cvImageChanged)



public:
    CVCamera(QQuickItem* parent = 0);

    ~CVCamera();

    int getDevice() const;

    void setDevice(int device);

    QSize getSize() const;

    void setSize(QSize size);

    QStringList getDeviceList() const;

    QAbstractVideoSurface* getVideoSurface() const;

    void setVideoSurface(QAbstractVideoSurface* videoSurface);

    cv::Mat getCvImage();


    int getFrameNow() const;
    int getTotalNow() const;
    int getNoHatNow() const;

    void setFrameNow(int f);

signals:
    void sizeChanged();

    void deviceListChanged();

    void cvImageChanged();

    void frameNowChanged();
    void totalNowChanged();
    void noHatNowChanged();

private:
    const QVideoFrame::PixelFormat VIDEO_OUTPUT_FORMAT = QVideoFrame::PixelFormat::Format_ARGB32;

    bool isrunning = true;
    int device = -1;
    QStringList deviceList;
    QSize size;

    double fps;
    int frameNow = 0;
    int totalNow = 0;
    int noHatNow = 0;
    std::vector<bbox_t> results;

    QVariantList qFrameNow, qBoxID, qObjID, qX, qY, qW, qH, qProb;  // Database variant
    QString fileUrl;




    BetterVideoCapture* camera = NULL;                      ///< The camera object
    CameraThread* thread = NULL;                            ///< Thread to run camera image acquisition


    QVideoFrame* videoFrame = NULL;
    QAbstractVideoSurface* videoSurface = NULL;


    bool exportCvImage = false;
    cv::Mat cvImage;
    unsigned char* cvImageBuf = NULL;

    void update();
    void allocateCvImage();
    void allocateVideoFrame();
    void clearQVariant();
    bool saveDatabase();


public slots:

    void changeParent(QQuickItem* parent);
    void videoControl();


private slots:

    void imageReceived();


};

#endif // CVCAMERA_H
