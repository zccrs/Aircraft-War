import QtQuick 2.0
import Sailfish.Silica 1.0

Page{
    Item{
        id: main

        property int quit_y: quit.y
        property bool appActive: Qt.application.active
        anchors.centerIn: parent
        height: parent.height
        Component.onCompleted: {
            if(parent.width>parent.height){
                width = parent.height/840*480
            }else{
                width = parent.width
            }
        }

        onAppActiveChanged:
        {
            if(appActive)
            {
                if(gameEntity.running)
                    startMovie()
            }
            else
                pauseMovie()
        }

        function pauseMovie()
        {
            if(go1.running)
                go1.pause()
            else if(go2.running)
                go2.pause()
        }
        function startMovie()
        {
            if(go1.running)
                go1.resume()
            else if(go2.running)
                go2.resume()
            else
                go1.start()
        }

        function backButton(){
            music.start_music("button")
            if(double_text.text == "再按一次退出")
            {
                Qt.quit()
            }else{
                double_text.text = "再按一次退出"
                time_quit.start()
            }
        }

        function showMessage(message){
            messageDialog.text = message
            messageDialog.open()
        }
        Timer{
            id:time_quit
            interval: 2000
            onTriggered: double_text.text = "双击屏幕开始游戏"
        }
        Music{id:music}//音乐播放

        Image {
            id: main_back1

            width: parent.width
            height: parent.height
            source: width>height?"qrc:/Image/Blackground_Symbian.png":"qrc:/Image/Blackground_MeeGo.png"
            anchors.horizontalCenter: parent.horizontalCenter
            NumberAnimation{
                id:go1
                target: main_back1
                properties: "y"
                running: false
                from:0
                to:main_back1.height
                duration: 20000
                onStopped: go2.start()
            }
            onYChanged:
                if(go1.running)
                    main_back2.y=main_back1.y-height
        }
        Image {
            id: main_back2

            width: parent.width
            height: parent.height
            source: "qrc:/Image/Blackground_MeeGo.png"
            anchors.horizontalCenter: parent.horizontalCenter
            NumberAnimation{
                id:go2
                target: main_back2
                properties: "y"
                from: 0
                running: false
                to:main_back2.height
                duration: 20000
                onStopped: go1.start()
            }
            onYChanged:
                if(go2.running)
                    main_back1.y=main_back2.y-height
        }

        Item
        {
            id:start_main

            width: parent.width
            height: parent.height
            visible: opacity>0
            MouseArea
            {
                anchors.fill: parent
                onDoubleClicked:
                {
                    glint.stop()
                    load.start()
                }
            }
            Timer{
                id:load
                interval: 200
                repeat: true
                property int count_load: 0
                onTriggered:
                {
                    if(count_load==0)
                    {
                        load1.visible=true
                        count_load++
                    }

                    else if(count_load==1)
                    {
                        load1.visible=false
                        load2.visible=true
                        count_load++
                    }

                    else if(count_load==2)
                    {
                        load2.visible=false
                        load3.visible=true
                        count_load++
                    }
                    else
                    {
                        load.stop()
                        if(!rankEntity.opacity&!settingEntity.opacity)
                        {
                            music.start_music("button")
                            start_main.opacity=0
                            gameEntity.opacity=1
                            glint.stop()
                            gameEntity.start_game()
                        }
                        load3.visible=false
                        count_load=0
                    }
                }
            }

            Image{
                id:load1

                source: "qrc:/Image/loading_01.png"
                smooth: true
                width: sourceSize.width*main.width/480
                height: sourceSize.height*main.width/480
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: logo.bottom
                anchors.topMargin: 10
                visible: false
            }
            Image{
                id:load2

                source: "qrc:/Image/loading_02.png"
                smooth: true
                anchors.fill: load1
                visible: false
            }
            Image{
                id:load3

                width: parent.width/2
                source: "qrc:/Image/loading_03.png"
                smooth: true
                anchors.fill: load2
                visible: false
            }
            Image {
                id: logo

                source: "qrc:/Image/LOGO.png"
                x:0
                y:7*parent.height/24
                scale: main.width/480
                smooth: true
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Item{
                id:beginGame

                width:double_text.width
                height: double_text.height
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: load1.bottom
                anchors.topMargin: 10
                property string font_names:"00"

                FontLoader {
                    id: localFont
                    source: "file:///usr/share/harbour-aircraftwar/data/fzmw.ttf"
                }
                Text{
                    id:double_text

                    color: "#303030"
                    font.family: localFont.name
                    font.pointSize: 18
                    font.bold: true
                    text:"双击屏幕开始游戏"
                }

                Timer{
                    id:glint
                    interval: 500
                    repeat: true
                    onTriggered:
                    {
                        if(beginGame.visible==true)
                            beginGame.visible=false
                        else beginGame.visible=true
                    }
                    onRunningChanged:
                    {
                        if(running)
                            double_text.visible=true
                        else
                            double_text.visible=false
                    }
                }
            }

            Image {
                id: rank

                width: sourceSize.width*main.width/480
                height: sourceSize.height*main.width/480
                source: "qrc:/Image/Button_rank.png"
                y:unfold.y
                x:unfold.x
                visible:false
                smooth: true
                Behavior on y{
                    NumberAnimation{duration: 200}
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:
                    {
                        music.start_music("button")
                        unfold.changedUnfold()
                        glint.stop()
                        rankEntity.opacity=1
                        start_main.opacity=0
                        opacity=1
                        rankEntity.resetRank( true )//刷新排行榜
                    }
                }
            }
            Image {
                id: setting

                width: sourceSize.width*main.width/480
                height: sourceSize.height*main.width/480
                source: "qrc:/Image/Button_setting.png"
                y:unfold.y
                x:unfold.x
                visible:false
                smooth: true
                Behavior on y{
                    NumberAnimation{duration: 200}
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:
                    {
                        music.start_music("button")
                        unfold.changedUnfold()

                        glint.stop()
                        settingEntity.opacity=1
                        start_main.opacity=0
                    }
                }
            }
            Image {
                id: about

                width: sourceSize.width*main.width/480
                height: sourceSize.height*main.width/480
                source: "qrc:/Image/Button_info.png"
                y:unfold.y
                x:unfold.x
                smooth: true
                visible:false
                Behavior on y{
                    NumberAnimation{duration: 200}
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:
                    {
                        music.start_music("button")
                        unfold.changedUnfold()
                        glint.stop()
                        aboutEntity.opacity=1
                        start_main.opacity=0
                    }

                }
            }
            Image{
                id:unfold

                width: sourceSize.width*main.width/480
                height: sourceSize.height*main.width/480
                source: "qrc:/Image/Button_unfold.png"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 30
                anchors.bottomMargin: 20

                Behavior on rotation {
                    PropertyAnimation{duration: 200}
                }
                function stretch()
                {
                    rank.visible=true
                    about.visible=true
                    setting.visible=true
                    //button_center.visible=true

                    about.y=unfold.y-about.height-main.height/35
                    setting.y=unfold.y-about.height-setting.height-2*main.height/35
                    rank.y=unfold.y-about.height-setting.height-rank.height-3*main.height/35
                    //button_center.y = unfold.y-about.height-setting.height-rank.height-button_center.height-4*main.height/35
                }
                function shrink()
                {
                    about.y=unfold.y
                    setting.y=unfold.y
                    rank.y=unfold.y
                    //button_center.y=unfold.y
                }
                function changedUnfold()
                {

                    if(unfold.rotation==0)
                    {
                        unfold.rotation=45
                        unfold.stretch()
                    }
                    else
                    {
                        unfold.rotation=0
                        unfold.shrink()
                    }
                }
                onRotationChanged:
                    if(rotation==0)
                    {
                        rank.visible=false
                        about.visible=false
                        setting.visible=false
                        //button_center.visible=false
                    }

                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        music.start_music("button")
                        unfold.changedUnfold()
                    }
                }
            }

            Image{
                id:quit
                smooth: true
                source: "qrc:/Image/Button_back.png"
                x:main.width/18
                width: sourceSize.width*main.width/480
                height: sourceSize.height*main.width/480
                anchors.verticalCenter: unfold.verticalCenter

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        main.backButton()
                    }
                }
            }
        }

        About{
            id:aboutEntity
            function keyAction()
            {
                start_main.opacity=1
                opacity=0
                music.start_music("button")
            }
        }
        Setting{
            id:settingEntity
            function keyAction()
            {
                start_main.opacity=1
                opacity=0
                music.start_music("button")
            }
        }
        Game{
            id:gameEntity

            function keyAction()
            {
                start_main.opacity=1
                opacity=0
                music.start_music("button")
            }
        }
        Rank{
            id:rankEntity

            function keyAction()
            {
                opacity=0
                start_main.opacity=1
                music.start_music("button")
            }
        }
    }
}
