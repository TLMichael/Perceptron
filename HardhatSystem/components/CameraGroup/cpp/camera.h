#ifndef GROUP_CAMERAGROUP_CPP_CAMERA_H_
#define GROUP_CAMERAGROUP_CPP_CAMERA_H_

// JQToolsLibrary import
#include <JQToolsLibrary>

#define CAMERA_INITIALIZA                                               \
{                                                                       \
    qmlRegisterType<Video::Manage>("Camera", 1, 0, "CameraManage"); \
}

namespace Camera
{

class Manage: public AbstractTool
{
    Q_OBJECT
    Q_DISABLE_COPY(Manage)

public:
    Manage() = default;

    ~Manage() = default;

public slots:
    void openGitHubLink();

    void openLatestVersionDownloadLink();
};

}

#endif//GROUP_CAMERAGROUP_CPP_CAMERA_H_
