// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import "../general"
import "qrc:/i18n/des.js" as Mydes

Item{
    id:main
    width: 360
    height: 640
    property alias quit_y: quit.y
    property bool appActive: Qt.application.active//true//
    Music{id:music}//音乐播放
    Timer{
        interval: 300
        running: true
        onTriggered: {
            go1.start()
            //glint.start()
        }
    }

    onAppActiveChanged:
    {
        if(appActive)
        {
            if(gameEntity.opacity)
            {
                if(gameEntity.running)
                    main.startMovie()
            }
            else main.startMovie()
        }
        else main.pauseMovie()
    }
    function pauseMovie()
    {
        if(go1.running) go1.pause()
        else go2.pause()
    }
    function startMovie()
    {
        if(go1.running) go1.resume()
        else go2.resume()
    }
    Image {
        id: main_back1
        source: "qrc:/Image/Blackground_Symbian.png"
        anchors.horizontalCenter: parent.horizontalCenter
        NumberAnimation{
            id:go1
            target: main_back1
            properties: "y"
            from:0
            to:main_back1.height
            duration: main.height==854?20000:15000
            onCompleted: go2.start()
        }
        onYChanged:
            if(go1.running)
                main_back2.y=main_back1.y-height
    }
    Image {
        id: main_back2
        source: "qrc:/Image/Blackground_Symbian.png"
        anchors.horizontalCenter: parent.horizontalCenter
        NumberAnimation{
            id:go2
            target: main_back2
            properties: "y"
            from: 0
            to:main_back2.height
            duration: main.height==854?20000:15000
            onCompleted: go1.start()
        }
        onYChanged:
            if(go2.running)
                main_back1.y=main_back2.y-height
    }
    Item
    {
        id:start_main
        //visible: true
        width: parent.width
        height: parent.height
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
                        console.log("game button ok")
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
            //scale:main.height==854?1:0.75
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: logo.bottom
            anchors.topMargin: 10
            visible: false
        }
        Image{
            id:load2
            source: "qrc:/Image/loading_02.png"
            //scale:main.height==854?1:0.75
            smooth: true
            anchors.fill: load1
            visible: false
        }
        Image{
            id:load3
            source: "qrc:/Image/loading_03.png"
            //scale:main.height==854?1:0.75
            smooth: true
            anchors.fill: load2
            visible: false
        }
        Image {
            id: logo
            source: "qrc:/Image/LOGO.png"
            x:0
            y:7*parent.height/24
            smooth: true
            scale: main.height==854?1:0.75
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
                source: "qrc:/fzmw.ttf"
            }
            Text{
                id:double_text
                color: "#303030"
                font.family: localFont.name
                font.pixelSize: 22
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
            source: "qrc:/Image/Button_rank.png"
            y:unfold.y
            x:unfold.x
            visible:false
            smooth: true
            scale: main.height==854?1:0.75
            Behavior on y{
                NumberAnimation{duration: 200}
            }
            MouseArea{
                anchors.fill: parent
                onClicked:
                {
                    console.log("rank button ok")
                    music.start_music("button")
                    unfold.changedUnfold()
                    glint.stop()
                    rankEntity.opacity=1
                    start_main.opacity=0
                    opacity=1
                }
            }
        }
        Image {
            id: setting
            source: "qrc:/Image/Button_setting.png"
            y:unfold.y
            x:unfold.x
            visible:false
            smooth: true
            scale: main.height==854?1:0.75
            Behavior on y{
                NumberAnimation{duration: 200}
            }
            MouseArea{
                anchors.fill: parent
                onClicked:
                {
                    console.log("button setting ok")
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
            source: "qrc:/Image/Button_info.png"
            y:unfold.y
            x:unfold.x
            smooth: true
            scale: main.height==854?1:0.75
            visible:false
            Behavior on y{
                NumberAnimation{duration: 200}
            }
            MouseArea{
                anchors.fill: parent
                onClicked:
                {
                    console.log("button about ok")
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
            smooth: true
            scale: main.height==854?1:0.75
            source: "qrc:/Image/Button_unfold.png"

            x:parent.width-width-main.width/18
            y:parent.height-height-main.height/35

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
            scale: main.height==854?1:0.8
            anchors.verticalCenter: unfold.verticalCenter

            MouseArea{
                anchors.fill: parent

                onClicked: {
                    music.start_music("button")
                    if(double_text.text == "再按一次退出")
                    {
                        Qt.quit()
                    }else{
                        double_text.text = "再按一次退出"
                        time_quit.start()
                    }
                }
            }
            Timer{
                id:time_quit
                interval: 2000
                onTriggered: double_text.text = "双击屏幕开始游戏"
            }

        }
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }
    }
    About{
        id:aboutEntity
        function keyAction()
        {
            music.start_music("button")
            start_main.opacity=1
            opacity=0
            //glint.start()
        }
    }
    Setting{
        id:settingEntity
        function keyAction()
        {
            music.start_music("button")
            start_main.opacity=1
            opacity=0
            //glint.start()
        }
    }
    Game{
        id:gameEntity
        function keyAction()
        {
            music.start_music("button")
            start_main.opacity=1
            opacity=0
            //glint.start()
        }
    }
    Rank{
        id:rankEntity
        function keyAction()
        {
            music.start_music("button")
            opacity=0
            start_main.opacity=1
            //glint.start()
        }
    }
    Component.onCompleted: {

    }
}
