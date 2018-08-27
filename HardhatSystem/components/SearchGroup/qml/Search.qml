import QtQuick 2.7
import QtQuick.Controls 1.4
import QtMultimedia 5.4
import QtQuick.Dialogs 1.1
import "qrc:/MaterialUI/Interface/"

import com.nuaa.CModel 1.0
import Search 1.0
import com.nuaa.CVSearch 1.0


Item {
    id: video
    width: 820
    height: 590

    SearchManage {
        id: searchManage
    }

    VideoListModel {
        id: videoModel
    }

    onVisibleChanged: {
        if(video.visible === true)
            videoModel.reload()
    }





    Rectangle {
        id: videoRec
        anchors.fill: parent
        z: 1
        border.color: "#07083a"
        visible: false


        Rectangle {
            id: playerRec
            x: 0
            y: 0
            width: parent.width
            height:520
            color: "black"

            CVSearch {
                id: mediaPlayer
                fileUrl: ""

                onFrameNowChanged: {
                    totalNumLabel.text = mediaPlayer.totalNow
                    nohatNumLabel.text = mediaPlayer.noHatNow

                    pastBar.width = mediaPlayer.frameNow / mediaPlayer.getFrameCount() * parent.width
                    if(pastBar.width === parent.width)
                        controlbutton.text = "重新开始"

                    // console.log(pastBar.width)
                }

                onHeatMapChanged:{
                    console.log(heatMapBar.width)
                    heatMapBar.source = heatMapString
                }
            }

            VideoOutput {
                id: out
                height: 470
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.bottomMargin: 58
                anchors.fill: parent
                source: mediaPlayer
            }

            Image {
                id: heatMapBar
                x: 0
                y: 460
                width: parent.width
                height: 45
                z: 5

            }

            Rectangle {
                id: pastBar
                x: 0
                y: 500
                width: 0
                height: 20
                color: "orange"
                z: 1
            }

            Rectangle {
                id: futureBar
                x: 0
                y: 500
                width: parent.width
                height: 20
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
            id: controlbutton
            x: 692
            y: 532
            width: 120
            height: 40
            text: "暂停"

            onClicked: {
                if(text === "暂停")
                {
                    text = "继续"
                    mediaPlayer.videoControl()
                }
                else if(text === "继续")
                {
                    text = "暂停"
                    mediaPlayer.videoControl()
                }
                else
                {
                    text = "暂停"
                    mediaPlayer.videoControl()
                }

            }
        }
        MaterialButton {
            id: backbutton
            x: 550
            y: 532
            width: 120
            height: 40
            text: "返回"

            onClicked: {
                mediaPlayer.fileUrl = "0"
                //  videoModel.reload()
                videoRec.visible = false
                videoView.visible = true
            }
        }
        MaterialButton {
            id: txtbutton
            x: 408
            y: 532
            width: 120
            height: 40
            text: "导出数据"

            onClicked: {
                console.log("clicked!...............................................")
                searchManage.save(mediaPlayer.fileUrl)
                txtDialog.show("导出数据", "数据已被存入到：" + searchManage.getTxtName(), null, null)
            }
        }

        MaterialDialog {
            id: txtDialog
            z: 2

            // negativeButtonText: ("取消")
            positiveButtonText: ("确认")

            property var callbackOnCancel: null
            property var callbackOnOK: null

            function show(title, message, callbackOnCancel, callbackOnOK) {
                deleteDia.title = title
                deleteDia.text = message
                deleteDia.callbackOnCancel = callbackOnCancel
                deleteDia.callbackOnOK = callbackOnOK
                deleteDia.open()

                darkBackground.opacity = 1
            }

            onAccepted: {
                darkBackground.opacity = 0;

                if (callbackOnOK)
                {
                    callbackOnOK()
                }
            }

            onRejected: {
                darkBackground.opacity = 0;

                if (callbackOnCancel)
                {
                    callbackOnCancel()
                }
            }
        }

//        MessageDialog{
//            id: txtDialog
//            standardButtons: StandardButton.Yes
//            modality: Qt.ApplicationModal
//            title: qsTr("导出数据")
//            text: qsTr("数据已被存入到：" + mediaPlayer.fileUrl)
//            onYes: txtDialog.quit();
//        }
    }

    Component {
        id: videoDelegate
        Item {
            id: wrapper
            width: parent.width
            height: 60

            MouseArea {
                id: itemArea
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    wrapper.ListView.view.currentIndex = index
                }
                onClicked: {
                    var model = videoView.model
                    mediaPlayer.fileUrl = model.getPath(videoView.currentIndex)
                    console.log(model.getPath(videoView.currentIndex))

                    videoRec.visible = true
                    videoView.visible = false
                    controlbutton.text = "暂停"
                }
            }

            Row {
                id: videoRow
                spacing: video.width / 16 / 5

                Rectangle {

                    width: video.width / 16
                    height: wrapper.height
                    color: wrapper.ListView.isCurrentItem? "red" : "black"
                }

                Text {
                    id: pathText
                    width: video.width / 16 * 7
                    text: path
                    color: wrapper.ListView.isCurrentItem? "blue" : "black"
                    font.pixelSize: 18
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    width: video.width / 16
                    text: fps
                    color: wrapper.ListView.isCurrentItem? "blue" : "black"
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignHCenter
                }
                Text {
                    width: video.width / 16
                    text: frameCount
                    color: wrapper.ListView.isCurrentItem? "blue" : "black"
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignHCenter

                }
                Text {
                    width: video.width / 16 * 4
                    text: saveTime
                    color: wrapper.ListView.isCurrentItem? "blue" : "black"
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignHCenter

                }

                Rectangle {
                    id: deleteRec
                    width: video.width / 16
                    height: wrapper.height
                    color: wrapper.ListView.isCurrentItem? "red" : "black"

                    Label {
                        color: "#ffffff"
                        anchors.fill: parent
                        text: "删除"
                        font.pointSize: 15
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    MouseArea {
                        id: deleteArea
                        anchors.fill: parent
                        hoverEnabled: true

                        property string msgtext: ""
                        property string url: ""

                        onEntered: {
                            wrapper.ListView.view.currentIndex = index
                        }

                        onClicked: {
                            var model = videoView.model
                            url = model.getPath(videoView.currentIndex)
                            msgtext = url + "将被删除"

                            function accpet() {
                                console.log("accept")
                                var flag = model.deleteVideo(url)
                                if(flag === true)
                                    console.log("删除成功")
                                else
                                    console.log("删除失败")
                                videoModel.reload()
                            }

                            deleteDia.show("删除", msgtext, null, accpet)
                        }
                    }
                }
            }
        }
    }

    MaterialDialog {
        id: deleteDia
        z: 2

        negativeButtonText: ("取消")
        positiveButtonText: ("确认")

        property var callbackOnCancel: null
        property var callbackOnOK: null

        function show(title, message, callbackOnCancel, callbackOnOK) {
            deleteDia.title = title
            deleteDia.text = message
            deleteDia.callbackOnCancel = callbackOnCancel
            deleteDia.callbackOnOK = callbackOnOK
            deleteDia.open()

            darkBackground.opacity = 1
        }

        onAccepted: {
            darkBackground.opacity = 0;

            if (callbackOnOK)
            {
                callbackOnOK()
            }
        }

        onRejected: {
            darkBackground.opacity = 0;

            if (callbackOnCancel)
            {
                callbackOnCancel()
            }
        }
    }

    Rectangle {
        id: darkBackground
        anchors.fill: parent
        color: "#55000000"
        z: 1
        visible: opacity !== 0
        opacity: 0

        Behavior on opacity {
            NumberAnimation { duration: 500; easing.type: Easing.OutCubic }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons

            //            onClicked: {
            //                if (materialUI.onDarkBackgroundClicked)
            //                {
            //                    materialUI.onDarkBackgroundClicked();
            //                }
            //            }
        }
    }


    Rectangle {
        id: videoViewRec
        x: 0
        y: 61
        width: 820
        height: 529
        color: "#ffffff"

        ListView {
            id: videoView
            highlightRangeMode: ListView.NoHighlightRange
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.fill: parent
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            spacing: 7
            delegate: videoDelegate
            model: videoModel
            focus: true
            highlight: Rectangle{
                width: videoViewRec.width
                color: "lightblue"
            }
        }

    }

    Rectangle {
        id: labelRec
        x: 0
        y: 0
        width: 820
        height: 61
        color: "#ffffff"

        Label {
            id: label2
            x: 574
            y: 8
            width: 161
            height: 53
            text: qsTr("保存日期")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Courier"
            font.pointSize: 21
        }

        Label {
            id: label1
            x: 486
            y: 8
            width: 70
            height: 53
            text: qsTr("帧数")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Courier"
            font.pointSize: 21
        }

        Label {
            id: label
            x: 420
            y: 8
            width: 70
            height: 53
            text: qsTr("帧率")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "Courier"
            font.pointSize: 21
        }

        Label {
            id: label3
            x: 136
            y: 8
            width: 161
            height: 53
            text: qsTr("视频名称")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Courier"
            font.pointSize: 21
        }


    }





}
