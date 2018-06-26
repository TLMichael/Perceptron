import QtQuick 2.7
import QtQuick.Controls 1.4
import QtMultimedia 5.4
import QtQuick.Dialogs 1.0
import "qrc:/MaterialUI/Interface/"
import Video 1.0

import com.nuaa.CVVideo 1.0

Item {
    id: video
    width: 820
    height: 590

    VideoManage {
        id: videoManage
    }

    CVVideo {
        id: mediaPlayer
        fileUrl: ""

        onFrameNowChanged: {
            pastBar.width = mediaPlayer.frameNow / mediaPlayer.getFrameCount() * parent.width
            if(pastBar.width == parent.width)
                controlbutton.text = "重新开始"
            // console.log(pastBar.width)
        }

    }

    Rectangle {
        id: playerRect
        x: 0
        y: 0
        width: parent.width
        height: 520

        VideoOutput {
            id: out
            x: 0
            y: 0
            width: parent.width
            height: parent.height

            source: mediaPlayer
        }

        Rectangle {
            id: pastBar
            x: 0
            y: 510
            width: 0
            height: 10
            color: "orange"
            z: 1
        }

        Rectangle {
            id: futureBar
            x: 0
            y: 510
            width: parent.width
            height: 10
            color: "gray"
        }

    }

    MaterialButton {
        id: choicebuttom
        width: 120
        height: 40
        text: "选择视频"


        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        onClicked: {
            fileDialog.open()
            anchors.verticalCenterOffset = 261
            anchors.horizontalCenterOffset = 185
            controlbutton.visible = true

            playerRect.color = "black"
        }
    }

    FileDialog {
        id: fileDialog

        onAccepted: mediaPlayer.fileUrl = fileUrl
    }

    MaterialButton {
        id: controlbutton
        visible: false

        x: 8
        y: -2
        width: 120
        height: 40
        text: "暂停"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 261
        anchors.horizontalCenterOffset: 328

        onClicked: {
            if(text === "暂停")
                text = "继续"
            else
                text = "暂停"
            mediaPlayer.videoControl()
        }
    }


}
