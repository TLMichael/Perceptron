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
            totalNumLabel.text = mediaPlayer.totalNow
            nohatNumLabel.text = mediaPlayer.noHatNow
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
        visible: false

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

        Label {
            id: nohatNumLabel
            x: 756
            y: 52
            width: 37
            height: 32
            color: "#79d1fc"
            text: qsTr("0")
            z: 4
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            clip: false
            font.pointSize: 15
            opacity: 1
            font.family: "Courier"
            verticalAlignment: Text.AlignVCenter
        }

        Label {
            id: totalNumLabel
            x: 756
            y: 17
            width: 37
            height: 32
            color: "#79d1fc"
            text: qsTr("0")
            z: 3
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Courier"
            clip: false
            opacity: 1
            font.pointSize: 15
        }

        Label {
            id: nohatLabel
            x: 676
            y: 52
            width: 82
            height: 32
            color: "#82d5fd"
            text: qsTr("未戴帽：")
            z: 1
            font.bold: true
            clip: false
            opacity: 1
            font.pointSize: 15
        }

        Label {
            id: totalLabel
            x: 676
            y: 17
            width: 82
            height: 32
            color: "#79d1fc"
            text: qsTr("总人数：")
            z: 2
            font.bold: true
            font.pointSize: 15
            opacity: 1
            clip: false
        }

        Rectangle {
            id: shadowRec
            x: 657
            y: 8
            width: 155
            height: 86
            color: "#000000"
            radius: 0
            border.color: "#dc1c04"
            border.width: 5
            opacity: 0.5
        }

    }

    MaterialButton {
        id: choicebuttom
        width: 200
        height: 84
        text: qsTr("选择视频")
        textFontFamily: "Roboto"
        textSize: 35
        anchors.verticalCenterOffset: 0
        anchors.horizontalCenterOffset: 0


        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        onClicked: {
            fileDialog.open()
            anchors.verticalCenterOffset = 261
            anchors.horizontalCenterOffset = 185
            controlbutton.visible = true

            playerRect.color = "black"
            playerRect.visible = true;
        }
    }

    FileDialog {
        id: fileDialog

        onAccepted: {
            controlbutton.text = "暂停"
            mediaPlayer.fileUrl = fileUrl
        }
    }


    onVisibleChanged: {
        console.log("video visible changed! Visible: ", video.visible)
        if(video.visible === false)
        {
            if(controlbutton.text === "初始")
                controlbutton.text = "初始"
            else if(controlbutton.text === "暂停")
            {
                controlbutton.text = "继续"
                mediaPlayer.videoControl()
            }
        }
    }

    MaterialButton {
        id: controlbutton
        visible: false

        x: 8
        y: -2
        width: 120
        height: 40
        text: "初始"
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
