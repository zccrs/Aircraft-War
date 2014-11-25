// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Item
{
    id:settings_main
    width: parent.width
    height: parent.height
    opacity:0
    //property bool isGame: false
    Behavior on opacity {
        NumberAnimation{duration: 300}
    }
    function keyAction(){}

    Image{
        id:setting01
        source: "qrc:/Image/Setting_01.png"
        sourceSize.width: parent.height!=640?410:410*0.75
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Image{
        id:setting02
        source: "qrc:/Image/Setting_02.png"
        sourceSize.width: parent.height!=640?410:410*0.75
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
    }

    Column{
        id: column
        spacing: 20

        opacity: 1 - update_diglog.opacity
        anchors.top: setting01.bottom
        anchors.topMargin: 20
        width: setting01.width
        anchors.horizontalCenter: parent.horizontalCenter
        Item{
            width: setting01.width
            height: musicinfo_switch.height
            Text {
                anchors.left: parent.left
                text: "音乐"
                //verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: main.height!=640?26:22
                font.family: localFont.name
            }
            Flipable{
                id:musicinfo_switch
                width: parent.height!=640?70:50
                height: width
                anchors.right: parent.right
                //anchors.horizontalCenter: parent.horizontalCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked:
                    {
                        music.start_music("button")
                        if(mysettings.getValue("game_music_switch","true")=="true")
                        {
                            mysettings.setValue("game_music_switch","false")
                            rotation1.angle=180
                        }

                        else
                        {
                            mysettings.setValue("game_music_switch","true")
                            rotation1.angle=0
                        }
                    }
                }
                transform: Rotation {
                         id: rotation1
                         origin.x: musicinfo_switch.width/2
                         origin.y: musicinfo_switch.height/2
                         axis.x: 0; axis.y: 1; axis.z: 0
                         angle: mysettings.getValue("game_music_switch","true")=="true"?0:180
                         Behavior on angle{
                             NumberAnimation{duration: 200}
                         }
                     }

                front:Image{source: "qrc:/Image/true.png";smooth: true;anchors.fill:parent}
                back:Image{source: "qrc:/Image/false.png";smooth: true;anchors.fill:parent}
            }
        }

        Item{
            width: setting01.width
            height: sound_switch.height
            Text {
                anchors.left: parent.left
                text: "音效"
                //verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: main.height!=640?26:22
                font.family: localFont.name
            }
            Flipable{
                id:sound_switch
                //anchors.horizontalCenter: parent.horizontalCenter
                width: musicinfo_switch.width
                height: width
                anchors.right: parent.right
                MouseArea{
                    anchors.fill: parent
                    onClicked:
                    {
                        music.start_music("button")
                        if(mysettings.getValue("sound_switch","true")=="true")
                        {
                            mysettings.setValue("sound_switch","false")
                            rotation2.angle=180
                        }

                        else
                        {
                            mysettings.setValue("sound_switch","true")
                            rotation2.angle=0
                        }
                    }
                }
                transform: Rotation {
                         id: rotation2
                         origin.x: sound_switch.width/2
                         origin.y: sound_switch.height/2
                         axis.x: 0; axis.y: 1; axis.z: 0
                         angle: mysettings.getValue("sound_switch","true")=="true"?0:180
                         Behavior on angle{
                             NumberAnimation{duration: 200}
                         }
                     }

                front:Image{source: "qrc:/Image/true.png";smooth: true;anchors.fill:parent}
                back:Image{source: "qrc:/Image/false.png";smooth: true;anchors.fill:parent}
            }

        }
    }

    Image{
        id:find_new_appVersion
        opacity: 1- (user_center.opacity > update_diglog.opacity ? user_center.opacity: update_diglog.opacity)
        source:find_new_appVersion_mouse.pressed?"qrc:/Image/button_3_2.png":"qrc:/Image/button_3_1.png"
        scale:parent.height!=640?1:0.75
        smooth: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom:  my_account.top
        anchors.bottomMargin: 20
        HttpRequest{
            id: updateApp
            onPostFinish: {
                var jsondata = JSON.parse(reData)
                if( jsondata.version ){
                    utility.console("最新版本："+jsondata.version)
                    if( utility.appVersion != jsondata.version )
                        update_diglog.open( jsondata.version, jsondata.url )
                    else
                        update_diglog.open( "已经是最新版本", "" )
                }else
                    update_diglog.open( jsondata.message, "" )
            }
        }

        MouseArea{
            id:find_new_appVersion_mouse
            anchors.fill: parent
            onClicked:{
                music.start_music("button")
                var url = "http://api.9smart.cn/app/5"
                updateApp.post ("GET", url)
            }
        }
        Text{
            anchors.centerIn: parent
            font.family: localFont.name
            font.pixelSize: parent.height!=640?32:24
            text: "<b>检测更新</b>";
            color: "#303030"
        }
    }

    Image{
        id:my_account
        opacity: 1 - update_diglog.opacity
        source:my_account_mouse.pressed?"qrc:/Image/button_3_2.png":"qrc:/Image/button_3_1.png"
        scale:parent.height!=640?1:0.75
        smooth: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.width>480 ? setting02.bottom :setting02.top
        anchors.bottomMargin: 20
        MouseArea{
            id:my_account_mouse
            anchors.fill: parent
            onClicked:{
                music.start_music("button")
                setting01.visible=false
                setting02.visible=false
                column.visible=false
                my_account.visible=false
                user_center.opacity = 1
            }
        }
        Text{
            anchors.centerIn: parent
            font.family: localFont.name
            font.pixelSize: parent.height!=640?32:24
            text: "<b>个人中心</b>";
            color: "#303030"
        }
    }

    Image{
        id:quit
        opacity: 1 - update_diglog.opacity
        source: "qrc:/Image/Button_back.png"
        x:main.width/18
        scale: parent.height!=640?1:0.75
        y:main.quit_y
        smooth: true
        MouseArea{
            anchors.fill: parent
            onClicked:
            {
                music.start_music("button")
                keyAction();
            }
        }
    }
    Image{
        id: update_diglog
        opacity: 0
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }
        source: "qrc:/Image/button_1.png"
        anchors.centerIn: parent
        sourceSize.width: parent.width-40
        property string appDownloadUrl: ""
        clip:true
        function open(ver,  downloadUrl ){
            appDownloadUrl = downloadUrl
            if( downloadUrl!="" ){
                dialog_text.text = "新版："+ver+"\n请使用久店下载！"
            }else{
                dialog_text.text = ver
            }
            update_diglog.opacity = 1
        }
        Text{
            id: title
            font.family: localFont.name
            anchors.horizontalCenter: parent.horizontalCenter
            y:20
            text:"提示"
            font.pixelSize: main.height!=640?34:28
        }
        Text{
            id: dialog_text
            font.family: localFont.name
            anchors.centerIn: parent
            font.pixelSize: main.height!=640?34:26
        }

        Image{
            visible: update_diglog.appDownloadUrl !=""
            source:no_download.pressed? "qrc:/Image/button_2_2.png": "qrc:/Image/button_2_1.png"
            scale: main.height!=640?1:0.75
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            Text {
                anchors.centerIn: parent
                text: "确定"
                font.family: localFont.name
                font.pixelSize: main.height!=640?34:26
            }
            MouseArea{
                id: no_download
                anchors.fill: parent
                onClicked: {
                    music.start_music("button")
                    update_diglog.opacity = 0
                }
            }
        }
        Image{
            visible: update_diglog.appDownloadUrl ==""
            source:finish_button.pressed? "qrc:/Image/button_2_2.png": "qrc:/Image/button_2_1.png"
            scale: main.height!=640?1:0.75
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            Text {
                anchors.centerIn: parent
                text: "完成"
                font.family: localFont.name
                font.pixelSize: main.height!=640?34:26
            }
            MouseArea{
                id: finish_button
                anchors.fill: parent
                onClicked: {
                    music.start_music("button")
                    update_diglog.opacity = 0
                }
            }
        }
    }
    UserCenter{
        id:user_center

        function keyAction()
        {
            user_center.opacity = 0
            setting01.visible=true
            setting02.visible=true
            column.visible=true
            my_account.visible=true
        }
    }
}
