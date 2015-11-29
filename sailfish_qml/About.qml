// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

Item
{
    id:aboutmain

    width: parent.width
    height: parent.height
    opacity:0
    visible: opacity>0
    Behavior on opacity {
        NumberAnimation{duration: 200}
    }
    function keyAction(){}
    Image{
        id:quit
        source: "qrc:/Image/Button_back.png"
        x:main.width/18
        width: sourceSize.width*main.width/480
        height: sourceSize.height*main.width/480
        y:main.quit_y

        MouseArea{
            anchors.fill: parent
            onClicked: keyAction()
        }
    }

    Image{
        id: about01

        width: sourceSize.width*main.width/480
        height: sourceSize.height*main.width/480
        source: "qrc:/Image/About_01.png"
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Image{
        id:about02

        width: sourceSize.width*main.width/480
        height: sourceSize.height*main.width/480
        source: "qrc:/Image/About_02.png"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
    ListView{
        id:list
        clip: true
        width: parent.width
        anchors.top: about01.bottom
        anchors.bottom: about02.top
        maximumFlickVelocity: 5000
        model: ListModel{
            ListElement{
            }
        }
        delegate: Image{
            width: sourceSize.width*main.width/480
            height: sourceSize.height*main.width/480
            source: "qrc:/Image/info_sailfish.png"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
