// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import planes 1.0
import "qrc:/i18n/des.js" as Mydes
Item
{
    id:game_main
    width: parent.width
    height: parent.height
    opacity:0
    property bool running: false
    property int myscore: 0//记录得分
    property string cacheScore:""

    Component.onCompleted: {
        mysettings.setValue("bomb",parseInt(0))//一定要初始化，不然得分不准确
        mysettings.setValue("myscore", parseInt(0))//一定要初始化，不然得分不准确
        var temp = mysettings.getValue("cacheScore","")
        if( temp != "" ){//如果上次有上传失败的分数
            var url="http://www.9smart.cn/topgame?appid=1"
            postScore.post("POST", url, temp)
        }
    }

    Behavior on opacity {
        NumberAnimation{duration: 200}
    }
    function keyAction(){}
    function start_game()
    {
        mysettings.setValue("myscore", parseInt(0))
        myscore=0
        game_main.running=true;
        planes.start_game()
        //console.log(planes.width+" "+planes.height)
        yidong.x=-yidong.width/4
        yidong.y=0
        planes.enabled=true

        //console.log(mouse.x+" "+mouse.y)
    }

    HttpRequest{
        id: postScore
        onPostFinish: {
            mysettings.setValue("user_score",parseInt(mysettings.getValue("myscore",parseInt(0))))
            mysettings.setValue("cacheScore","")//记得要设为空，不然下次还会post
            mysettings.setValue("cacheScoreInt",0)
            console.log(reData)
        }
        onError: {
            utility.console("上传分数出错")
            mysettings.setValue("cacheScore", cacheScore)
            mysettings.setValue("cacheScoreInt",parseInt(mysettings.getValue("myscore",parseInt(0))))
        }
    }

    function addScore(score)
    {
        utility.console( "两个记录分数的值分别是："+score+","+mysettings.getValue("myscore",parseInt(0)) )

        var temp = mysettings.getValue("cacheScore","")
        var temp1 = mysettings.getValue("cacheScoreInt",0)
        var uid = mysettings.getValue("user_uid","")

        if( temp!="" & temp1>score ){
            mysettings.setValue("myscore",parseInt(temp1))
            var postdata = temp
            console.log(postdata)
            cacheScore = postdata//记录要post的数据

            var url="http://www.9smart.cn/topgame?appid=1"
            postScore.post("POST", url, postdata)
        }else if(uid != "" & parseInt(score) > parseInt(mysettings.getValue("user_score",0) )){//
            if( parseInt(score) !=  parseInt(mysettings.getValue("myscore",parseInt(0))) )
                    return 0;
            var message = uid+","+String(mysettings.getValue("myscore",parseInt(0)));

            utility.console("将要发送的数据是："+message)
            var des_data = Mydes.des( message )

            var str_temp = utility.base64Encode(des_data)
            var temp_times = parseInt(planes.gameRuningTime/60000)
            temp_times = temp_times==0?1:temp_times
            var postdata = "{\"score\":\""+str_temp+"\",\"model\":\""+utility.productName+"\",\"time\":"+temp_times+"}"
            console.log(postdata)
            cacheScore = postdata//记录要post的数据

            var url="http://www.9smart.cn/topgame?appid=1"
            postScore.post("POST", url, postdata)
        }
    }

    Image{
        id:pause
        source:pausemouse.pressed?"qrc:/Image/Pause_02.png":"qrc:/Image/Pause_01.png"
        x:main.width/20
        y:main.height/30
        z:2
        scale: game_main.height!=640?1:0.8

        MouseArea{
            id:pausemouse
            anchors.fill: parent
            onClicked: {
                if(gameover.opacity==0)
                {
                    game_main.running=false
                    main.pauseMovie()
                    music.start_music("button")
                    music.stop_music("game_music")
                    planes.pause_game()
                    dialog_window.opacity=1

                    planes.enabled=false
                }
            }
        }
        smooth: true
    }

    Text {
        id: score
        font.family: localFont.name
        text:myscore==0?"0000":myscore
        x:pause.x+pause.width
        z:3
        anchors.verticalCenter: pause.verticalCenter
        font.pixelSize: parent.height!=640?35:31
        color: "black"
    }
    Item{
        id:dialog_window
        z:2
        width: 2*parent.width/3
        height: parent.height/3
        anchors.centerIn: parent
        opacity: 0
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }

        Image{
            id:quit
            anchors.horizontalCenter: parent.horizontalCenter
            source: mouse_quit.pressed?"qrc:/Image/button_3_2.png":"qrc:/Image/button_3_1.png"
            scale:main.height!=640?1:0.75
            smooth: true
            Text{
                color: "#303030"
                font.family: localFont.name
                font.pixelSize: main.height!=640?34:28
                text:"退出"
                anchors.centerIn: parent
            }

            MouseArea{
                id:mouse_quit
                anchors.fill: parent
                onClicked: {
                    main.startMovie()
                    planes.quit_game()
                    keyAction()
                    dialog_window.opacity=0
                    game_main.addScore(myscore)
                }
            }
        }

        Image{
            id:restart;
            anchors.horizontalCenter: parent.horizontalCenter
            y:quit.height+quit.y+parent.height/10
            source: restartmouse.pressed?"qrc:/Image/button_3_2.png":"qrc:/Image/button_3_1.png"
            scale:main.height!=640?1:0.75
            smooth: true
            Text{

                color: "#303030"
                font.family: localFont.name
                font.pixelSize: main.height!=640?34:28
                text:"重新开始"
                anchors.centerIn: parent
            }

            MouseArea{
                id:restartmouse
                anchors.fill: parent
                onClicked: {
                    game_main.addScore(myscore)
                    music.start_music("button")
                    planes.quit_game()
                    dialog_window.opacity=0
                    main.startMovie()
                    start_game()
                }
            }
        }

        Image{
            id:restore
            anchors.horizontalCenter: parent.horizontalCenter
            y:restart.height+restart.y+parent.height/10
            source: restoremouse.pressed?"qrc:/Image/button_3_2.png":"qrc:/Image/button_3_1.png"
            scale:main.height!=640?1:0.75
            smooth: true
            Text{

                color: "#303030"
                font.family: localFont.name
                font.pixelSize: main.height!=640?34:28
                text:"继续"
                anchors.centerIn: parent
            }

            MouseArea{
                id:restoremouse
                anchors.fill: parent
                onClicked: {
                    main.startMovie()
                    music.start_music("button")
                    planes.resume_game()
                    dialog_window.opacity=0
                    running=true
                    state=true
                    planes.enabled=true
                }
            }
        }


    }
    Image{
        id:gameover
        z:2
        anchors.centerIn: parent
        source: "qrc:/Image/button_1.png"
        scale: main.height!=640?1:0.75
        smooth: true
        opacity: 0
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }
        Rectangle{
            anchors.fill: parent
            radius: 11
            opacity: 0.2
            color: "black"
        }

        Text {
            color: "#303030"
            font.family: localFont.name
            font.pixelSize: main.height==854?42:36
            text:"飞机大战分数"
            anchors.horizontalCenter: parent.horizontalCenter
            y:parent.height/7-height/2

        }

        Text {
            color: "#303030"
            font.family: localFont.name
            font.pointSize: main.height==854?44:20
            text:myscore==0?"0000":myscore
            anchors.centerIn: parent
        }
        Image{
            id:gamequit
            source:gamequitmouse.pressed? "qrc:/Image/button_2_2.png": "qrc:/Image/button_2_1.png"
            scale: main.height!=640?1:0.75
            smooth: true
            x:parent.width/4-width/2
            y:6*parent.height/7-height/2
            Text{
                color: "#303030"
                font.family: localFont.name
                font.pixelSize: main.height==854?34:28
                text:"退出"
                anchors.centerIn: parent

            }

            MouseArea{
                id:gamequitmouse
                anchors.fill: parent
                onClicked: {
                    main.startMovie()
                    planes.quit_game()
                    keyAction()
                    gameover.opacity=0
                }
            }
        }

        Image{
            source: mouse1.pressed? "qrc:/Image/button_2_2.png": "qrc:/Image/button_2_1.png"
            scale: main.height!=640?1:0.75
            smooth: true
            x:3*parent.width/4-width/2
            y:gamequit.y
            Text{
                color: "#303030"
                font.family: localFont.name
                font.pixelSize: main.height==854?34:28
                text:"再玩一次"
                anchors.centerIn: parent
            }
            MouseArea{
                id:mouse1
                anchors.fill: parent
                onClicked: {
                    music.start_music("button")
                    planes.quit_game()
                    gameover.opacity=0
                    main.startMovie()
                    start_game()
                }
            }
        }
    }

    Image{
        id:mybomb
        source: "qrc:/Image/Bomb.png"
        scale: main.height!=640?0.8:0.6
        x:main.width/30
        y:main.height-height-x
        visible: planes.bomb>0
        z:3
        Text {
            id: bombCount
            x:parent.x+parent.width
            font.pointSize: main.height==854?50:16
            anchors.verticalCenter: mybomb.verticalCenter
            text: "x"+planes.bomb
            font.family: localFont.name
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {
                music.start_music("use_bomb")
                mysettings.setValue("bomb",parseInt(mysettings.getValue("bomb",parseInt(0)))-parseInt(1))
                planes.bomb = parseInt(mysettings.getValue("bomb",parseInt(0)));
                planes.pressBomb();//调用c++中的函数
            }
        }
    }

    Myplanes{
        id:planes
        anchors.fill: parent
        property bool game_state: running&Qt.application.active
        property int bomb: 0//炸弹道具的数量
        onGame_stateChanged:
        {
            //console.log("game_state changed:"+game_state)
            if(game_state==true)
            {
                //console.log("game_state=true")
                music.start_music("game_music")
                planes.resume_game()
            }

            else
            {
                planes.pause_game()
                music.stop_music("game_music")
            }
        }
        opacity: game_main.opacity
        onPlay_music:
        {
            music.start_music(music_name);
        }
        onAddScore: {
            mysettings.setValue("myscore",parseInt(mysettings.getValue("myscore",parseInt(0)))+parseInt(num))
            myscore += parseInt(num);
        }

        onGame_over: {
            game_main.running=false
            main.pauseMovie()
            pause.source="qrc:/Image/Pause_02.png"

            music.stop_music("game_music")
            gameover.opacity=1

            planes.enabled=false
            mysettings.setValue("bomb",parseInt(0))
            bomb=0
            game_main.addScore(myscore)
        }
        property bool jieshou: true
        Timer{
            id:timer_bomb
            interval: 5000
            onTriggered:{
                planes.jieshou=true
                console.log(planes.jieshou)
            }
        }
        onAcquireBomb:
        {
            if(jieshou)
            {
                jieshou=false
                music.start_music("bomb")
                if(mysettings.getValue("bomb",parseInt(0))<3)
                {
                    mysettings.setValue("bomb",parseInt(mysettings.getValue("bomb",parseInt(0)))+parseInt(1))
                    bomb = mysettings.getValue("bomb",parseInt(0));
                }
                timer_bomb.start()
            }
        }
    }
    MouseArea{
        id:yidong
        width: main.width*2
        height:main.height*2
        property real planeWidth: planes.planeWidth
        property real planeHeight: planes.planeHeight
        enabled: dialog_window.opacity==0?gameover.opacity==0?true:false:false
        x:-width/4
        y:0
        drag.axis: Drag.XandYAxis
        drag.target: yidong
        drag.maximumX: -planeWidth/2
        drag.maximumY: 0
        drag.minimumX: planeWidth/2-game_main.width
        drag.minimumY: planeHeight-game_main.height
        onMousePositionChanged: {
            planes.setPlanesXY(x-drag.minimumX,y-drag.minimumY)
        }
        onDoubleClicked:
        {
            if(mysettings.getValue("bomb",parseInt(0))>0)
            {
                music.start_music("use_bomb")
                mysettings.setValue("bomb",parseInt(mysettings.getValue("bomb",parseInt(0)))-parseInt(1))
                planes.bomb = mysettings.getValue("bomb",parseInt(0));
                planes.pressBomb();//调用c++中的函数
            }
        }
    }
}
