import QtQuick 2.7
import QtQuick.Controls 1.4
import QtMultimedia 5.4
import QtQuick.Dialogs 1.0
import "qrc:/MaterialUI/Interface/"

import com.nuaa.CModel 1.0
import Search 1.0


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

            MediaPlayer {
                id: mediaPlayer
                autoPlay: true

                onPositionChanged: {
                    pastBar.width = mediaPlayer.position / mediaPlayer.duration * parent.width
                }
                onStopped: {
                    controlbutton.text = "重新开始"
                }
            }

            VideoOutput {
                id: out
                anchors.fill: parent
                source: mediaPlayer
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
                    mediaPlayer.pause()
                }
                else if(text === "继续")
                {
                    text = "暂停"
                    mediaPlayer.play()
                }
                else
                {
                    text = "暂停"
                    mediaPlayer.seek(0)
                    mediaPlayer.play()
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
                mediaPlayer.pause()
                videoModel.reload()
                videoRec.visible = false
                videoView.visible = true
            }
        }
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
                    mediaPlayer.source = "file://" + model.getPath(videoView.currentIndex)
                    mediaPlayer.play()
                    console.log(model.getPath(videoView.currentIndex))
                    videoRec.visible = true
                    videoView.visible = false
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

                    width: video.width / 16
                    height: wrapper.height
                    color: wrapper.ListView.isCurrentItem? "red" : "black"
                }
            }

        }
    }

    ListView {
        id: videoView
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        spacing: 7
        delegate: videoDelegate
        model: videoModel
        focus: true
        highlight: Rectangle{
            width: parent.width
            color: "lightblue"
        }
    }




}
