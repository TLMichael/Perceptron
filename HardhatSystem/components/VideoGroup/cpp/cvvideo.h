/**
 * @file cvvideo.h
 * @brief Nice QML wrapper for video access with OpenCV
 */

#ifndef CVVIDEO_H
#define CVVIDEO_H

#include<QQuickItem>
#include<QCameraInfo>
#include<QList>
#include<QAbstractVideoSurface>
#include<QVideoSurfaceFormat>
#include<QSize>
#include<QMutex>
#include<QWaitCondition>
#include<QVariant>
#include<QSound>

#include<opencv2/highgui/highgui.hpp>

#include "videothread.h"
#include "utils.h"

#define CVVIDEO_INITIALIZA                                               \
{                                                                       \
    qmlRegisterType<CVVideo>("com.nuaa.CVVideo", 1, 0, "CVVideo"); \
}


class CVVideo : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(CVVideo)
    Q_PROPERTY(int frameNow READ getFrameNow WRITE setFrameNow NOTIFY frameNowChanged)
    Q_PROPERTY(int totalNow READ getTotalNow NOTIFY totalNowChanged)
    Q_PROPERTY(int noHatNow READ getNoHatNow NOTIFY noHatNowChanged)

    Q_PROPERTY(QString fileUrl READ getFileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getVideoSurface WRITE setVideoSurface)
    Q_PROPERTY(QSize size READ getSize WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(cv::Mat cvImage READ getCvImage NOTIFY cvImageChanged)





public:
    CVVideo(QQuickItem* parent = 0);

    ~CVVideo();

    QString getFileUrl() const;
    void setFileUrl(QString fileUrl);

    QSize getSize() const;

    void setSize(QSize size);

    QAbstractVideoSurface* getVideoSurface() const;

    void setVideoSurface(QAbstractVideoSurface* videoSurface);

    cv::Mat getCvImage();


    int getFrameNow() const;
    int getTotalNow() const;
    int getNoHatNow() const;

    void setFrameNow(int f);


signals:
    void frameNowChanged();
    void totalNowChanged();
    void noHatNowChanged();

    void sizeChanged();
    void fileUrlChanged();
    void cvImageChanged();

private:
    const QVideoFrame::PixelFormat VIDEO_OUTPUT_FORMAT = QVideoFrame::PixelFormat::Format_ARGB32;

    QString fileUrl = NULL;

    QSize size;
    double fps;
    int frameCount = 0;
    int frameNow = 0;
    int totalNow = 0;
    int noHatNow = 0;
    std::vector<bbox_t> results;


    QVariantList qFrameNow, qBoxID, qObjID, qX, qY, qW, qH, qProb;  // Database variant


    BetterVideoCapture* video = NULL;                      ///< The camera object
    VideoThread* thread = NULL;                            ///< Thread to run camera image acquisition


    QVideoFrame* videoFrame = NULL;
    QAbstractVideoSurface* videoSurface = NULL;


    bool exportCvImage = false;
    cv::Mat cvImage;
    unsigned char* cvImageBuf = NULL;

    bool isrunning = true;

    void update();
    void allocateCvImage();
    void allocateVideoFrame();
    void clearQVariant();


public slots:

    void changeParent(QQuickItem* parent);
    void videoControl();
    int getFrameCount();


private slots:
    void fileUrlGeted();
    void imageReceived();


};

#endif // CVVIDEO_H
