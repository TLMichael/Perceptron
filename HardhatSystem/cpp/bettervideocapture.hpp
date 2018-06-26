#ifndef BETTERVIDEOCAPTURE_H
#define BETTERVIDEOCAPTURE_H

#include<opencv2/videoio.hpp>
#include<QObject>

class BetterVideoCapture
{
public:
    BetterVideoCapture()
    {
        capture = new cv::VideoCapture();
    }

    virtual ~BetterVideoCapture()
    {
        delete capture;
    }

    bool open(int device)
    {
        return capture->open(device);
    }

    bool open(QString file)
    {
        return capture->open(file.toStdString());
    }

    double getProperty(int propIdx)
    {
        return capture->get(propIdx);
    }

    bool setProperty(int propIdx, double propVal)
    {
        return capture->set(propIdx,propVal);
    }


    bool grabFrame()
    {
        return capture->grab();
    }

    unsigned char* retrieveFrame()
    {
        capture->retrieve(rawImage);
        return rawImage.ptr();
    }

    bool isOpened()
    {
        return capture->isOpened();
    }

private:
    cv::VideoCapture* capture;
    cv::Mat rawImage;
};


#endif // BETTERVIDEOCAPTURE_H
