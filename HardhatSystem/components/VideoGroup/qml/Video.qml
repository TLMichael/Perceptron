import QtQuick 2.7
import QtQuick.Controls 1.4
import QtMultimedia 5.4
import QtQuick.Dialogs 1.0
import "qrc:/MaterialUI/Interface/"
import Video 1.0

Item {
    id: video
    width: 620
    height: 540

    VideoManage {
        id: videoManage
    }

    MediaPlayer {
        id: mediaPlayer
        autoPlay: true
    }

    VideoOutput {
        id: out
        anchors.fill: parent
        source: mediaPlayer
    }

    MaterialButton {
        width: 120
        height: 40
        text: "选择视频"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        onClicked: {
            fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        onAccepted: mediaPlayer.source = fileUrl
    }

}
