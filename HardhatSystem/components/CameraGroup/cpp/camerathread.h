#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include<QDebug>
#include<QThread>
#include<QObject>
#include<QElapsedTimer>
#include<QVideoFrame>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/videoio/videoio_c.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>

#include<vector>

#include "bettervideocapture.hpp"
#include "utils.h"


#define DPRINT(...) while(0);


/**
 * @brief Object that contains the camera loop and its parameters
 */
class CameraTask : public QObject{
    Q_OBJECT

public:

    /**
     * @brief Creates a new camera access task
     *
     * @param camera Camera object to get data from
     * @param videoFrame Place to draw the camera image, pass NULL to not draw camera image to a QVideoFrame
     * @param cvImageBuf Place to export the camera image, pass NULL to not export the camera image
     * @param width Width of the camera image
     * @param height Height of the camera image
     */
    CameraTask(std::vector<bbox_t> *results, BetterVideoCapture* camera, QVideoFrame* videoFrame, unsigned char* cvImageBuf, int width, int height);

    /**
         * @brief Destroys this camera access task, does not touch the camera or the videoFrame
         */
    virtual ~CameraTask();

    /**
     * @brief Asks for the main loop to stop
     */
    void stop();

private:

    int width;                                  ///< Width of the camera image
    int height;                                 ///< Height of the camera image
    BetterVideoCapture* camera;                 ///< The camera to get data from
    bool running = false;                       ///< Whether the worker thread is running
    QVideoFrame* videoFrame;                    ///< Place to draw camera image to
    unsigned char* cvImageBuf;                  ///< Place to export camera image to
    \
    std::vector<bbox_t> *results;

public slots:
    /**
         * @brief Continuously gets data from the camera
         */
    void doWork();


signals:

    /**
     * @brief Emitted when image from a new frame is ready
     */
    void imageReady();

};



/**
 * @brief Object that starts and stops the camera loop
 */
class CameraThread : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Creates a new camera controller
     *
     * @param camera Camera object to get data from
     * @param videoFrame Place to draw the camera image, pass NULL to not draw camera image to a QVideoFrame
     * @param cvImageBuf Place to export the camera image, pass NULL to not export the camera image
     * @param width Width of the camera image
     * @param height Height of the camera image
     */
    CameraThread(std::vector<bbox_t> *results, BetterVideoCapture* camera, QVideoFrame* videoFrame, unsigned char* cvImageBuf, int width, int height);

    /**
     * @brief Destroys this camera controller
     */
    virtual ~CameraThread();

    /**
     * @brief Starts the camera loop
     */
    void start();

    /**
     * @brief Asks the camera loop to stop
     */
    void stop();

private:

    QThread workerThread;               ///< The thread that the camera will work in
    CameraTask* task = NULL;            ///< The camera loop method and parameter container

signals:

    /**
     * @brief Emitted when image from a new frame is ready
     */
    void imageReady();


};

#endif // CAMERATHREAD_H
