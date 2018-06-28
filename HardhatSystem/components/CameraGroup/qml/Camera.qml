import QtQuick 2.7
import QtQuick.Controls 1.4
import QtMultimedia 5.4
import QtQuick.Dialogs 1.0
import "qrc:/MaterialUI/Interface/"
import Camera 1.0

import com.nuaa.CVCamera 1.0

Item {
    id: video
    width: 820
    height: 590

    CameraManage {
        id: cameraManage
    }

    CVCamera {
        id: mediaPlayer
        device: deviceBox.currentIndex
    }

    Rectangle {
        id: playerRect
        x: 0
        y: 0
        width: parent.width
        height:520
        color: "black"

        VideoOutput {
            id: out
            x: 0
            y: 0
            width: parent.width
            height: parent.height

            source: mediaPlayer
        }
    }

    MaterialButton {
        id: controlbutton
        x: 692
        y: 532
        width: 120
        height: 40
        text: "暂停"

        onClicked: {
            if(text === "暂停")
                text = "继续"
            else
                text = "暂停"
            mediaPlayer.videoControl()
        }
    }


    ComboBox {
        id: deviceBox
        x: 8
        y: 526
        width: 200
    }

}
