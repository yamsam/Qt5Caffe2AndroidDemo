import QtQuick 2.5
import QtMultimedia 5.5
import QtQuick.Controls 2.1
import Caffe2VideoFilterLib 1.0

ApplicationWindow {
    visible: true
    width: 270
    height: 480
    title: qsTr("Caffe2QtDemo")

    Camera {
        id: camera
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        autoOrientation: true
        source: camera
        filters: [ videoFilter ]
    }

    Caffe2VideoFilter {
        id: videoFilter
        orientation: videoOutput.orientation
        active: false
    }

    MouseArea {
        id : mousearea
        anchors.fill:parent
        onClicked: {
            videoFilter.active = !videoFilter.active
        }
    }

    Rectangle{
        width:  parent.width
        height: parent.height / 6
        color: 'black'

        anchors.top: parent.top

        Text {
            id: name
            text: videoFilter.result
            color: 'white'
        }
    }
}
