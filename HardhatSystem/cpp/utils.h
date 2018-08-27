#ifndef UTILS_H
#define UTILS_H

#include "yolo_v2_class.hpp"
#include "dataface.h"
#include <QSound>

extern Detector *detector;

static cv::Scalar obj_id_to_color(int obj_id);

extern DataFace *df;

extern cv::VideoWriter *videoWriter;

extern QSound *sound;

QString getCurrentTime();
QString getCurrentTime2();

QString getVideoDir();

#endif // UTILS_H
