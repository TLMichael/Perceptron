import QtQuick 2.7
import QtQuick.Controls 1.4
import QtMultimedia 5.4
import QtQuick.Dialogs 1.0
import "qrc:/MaterialUI/Interface/"
import Camera 1.0

Item {
    id: video
    width: 620
    height: 540

    CameraManage {
        id: cameraManage

    }

    Camera {
        id: camera
        imageCapture {
            onImageCaptured: {
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
    }

    Image {
        id: photoPreview
    }

    VideoOutput {
        id: out
        anchors.fill: parent
        source: camera
    }
}
