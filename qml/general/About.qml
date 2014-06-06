// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
Item
{
    id:aboutmain
    width: parent.width
    height: parent.height
    opacity:0
    Behavior on opacity {
        NumberAnimation{duration: 200}
    }
    function keyAction(){}
    Image{
        id:quit
        source: "qrc:/Image/Button_back.png"
        x:main.width/18
        scale: parent.height!=640?1:0.75
        y:main.quit_y
        smooth: true
        MouseArea{
            anchors.fill: parent
            onClicked: keyAction()
        }
    }

    Image{
        id: about01
        source: "qrc:/Image/About_01.png"
        sourceSize.width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Image{
        id:about02
        source: "qrc:/Image/About_02.png"
        sourceSize.width: parent.width
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
                info_meego:"qrc:/Image/info_0.png"
                info_symbian:"qrc:/Image/info_1.png"
            }
        }
        delegate: Image{
            source: aboutmain.height!=640?info_meego:info_symbian
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
