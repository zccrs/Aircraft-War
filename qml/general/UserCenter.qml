// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import QtWebKit 1.0

Item{
    id:user_center_main
    height: parent.height
    width: parent.width
    opacity:0

    onOpacityChanged:{
        if(opacity == 1)
        {
            var uid=mysettings.getValue("user_uid","")
            if(uid!="")
            {
                var url = "http://www.9smart.cn/topgame/mytop?appid=1&uid="+String(uid)
                post_ranking.post("GET",url)
            }
        }
    }

    function updataData()
    {
        user_avatar .source = mysettings.getValue("user_avatar","")
        user_nickname.text = "用户名："+mysettings.getValue("user_nickname","")

        var uid=mysettings.getValue("user_uid","")
        if(uid!="")
        {
            var url = "http://www.9smart.cn/topgame/mytop?appid=1&uid="+String(uid)
            post_ranking.post("GET",url)
        }
    }

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

    Image{
        id:user_avatar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: setting01.bottom
        anchors.topMargin: 10
        sourceSize.width: parent.width >480 ?80:100
        source: mysettings.getValue("user_avatar","")
    }

    Text {
        id: user_nickname
        text: "用户名："+mysettings.getValue("user_nickname","")
        anchors.top: user_avatar.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: main.height==854?26:18
        font.family: localFont.name
    }
    Text {
        id: user_phone_model
        anchors.top: user_nickname.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: main.height==854?26:18
        font.family: localFont.name
    }

    Text {
        id: user_ranking
        anchors.top: user_phone_model.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: main.height==854?26:18
        font.family: localFont.name
    }

    Text {
        id: user_score
        anchors.top: user_ranking.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: main.height==854?26:18
        font.family: localFont.name
    }
    HttpRequest{
        id: post_ranking
        onPostFinish: {
            console.log(reData)
            var jsondata = JSON.parse(reData)
            if(jsondata.error === 0)
            {
                user_phone_model.text = "玩家设备："+String(jsondata.model)
                user_ranking.text = "网络排行："+String(jsondata.top)
                user_score.text = "最新得分："+String(jsondata.score)
                mysettings.setValue("user_score",parseInt(jsondata.score))
            }
        }
        Component.onCompleted: updataData()
    }
    Image{
        id:quit
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
                keyAction()
            }
        }
    }

    Image{
        id:my_account
        source:my_account_mouse.pressed?"qrc:/Image/button_3_2.png":"qrc:/Image/button_3_1.png"
        scale:parent.height!=640?1:0.75
        smooth: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.width>480 ? setting02.bottom:setting02.top
        anchors.bottomMargin: 20
        MouseArea{
            id:my_account_mouse
            anchors.fill: parent
            onClicked:{
                music.start_music("button")
                webLogin.opacity=1
            }
        }
        Text{
            anchors.centerIn: parent
            font.family: localFont.name
            font.pixelSize: parent.height!=640?32:24
            text: "<b>更换账号</b>";
            color: "#303030"
        }
    }


    WebView{
        id:webLogin
        opacity: 0
        url:"http://www.9smart.cn/member/login"
        preferredHeight: settings_main.height
        preferredWidth:settings_main.width
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }
        property string html1: ""
        property string html2: ""
        property string html3: ""
        property bool isBack: false
        property int currentPage: 0

        function back()
        {
            switch(webLogin.currentPage)
            {
            case 1:
                return false
            case 2:
            {
                currentPage--
                isBack=true
                html = html1
                break;
            }

            case 3:
            {
                currentPage--
                isBack=true
                html = html2
                break
            }

            default:break
            }
            return true
        }

        onLoadFinished: {
            //console.log(currentPage+" , "+isBack)
            if(!webLogin.isBack){
                switch(webLogin.currentPage)
                {
                case 0:
                    webLogin.html1 = html
                    break
                case 1:
                    webLogin.html2 = html
                    break;
                default:break
                }
                webLogin.currentPage++
            }else webLogin.isBack=false

            var str = html//"<title>{\"uid\":\"5\",\"nickname\":\"\u2606\u96e8\u540e\u661f\u8fb0\u2606\",\"avatar\":\"http:\/\/q.qlogo.cn\/qqapp\/100387822\/56120F1DDEF5676BB4FB92F42DEC7B86\/40\"}</title>"

            var reg1 = new RegExp("<title>{.+}</title>")
            if(reg1.test(str))
            {
                var data1 = reg1.exec(str)
                var reg2 = new RegExp("{.+}")

                if(reg2.test(data1))
                {
                    var jsondata=JSON.parse(reg2.exec(data1))


                    if( jsondata ){
                        opacity = 0//登陆成功，返回游戏
                        mysettings.setValue("user_uid",jsondata.uid)
                        mysettings.setValue("user_nickname",jsondata.nickname)
                        mysettings.setValue("user_avatar",jsondata.avatar2)
                        mysettings.setValue("user_phone_model",jsondata.user_phone_model)

                        utility.console("user_uid："+mysettings.getValue("user_uid",""))
                        utility.console("user_nickname："+mysettings.getValue("user_nickname",""))
                        utility.console("user_avatar："+mysettings.getValue("user_avatar",""))
                        utility.console("user_phone_model"+mysettings.getValue("user_phone_model",""))

                        isBack = true
                        currentPage = 1
                        html = html1

                        updataData()//更新用户信息
                    }
                }
            }
        }
    }
    Image{
        id:back
        opacity: webLogin.opacity
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
                if(!webLogin.back())
                    webLogin.opacity=0
            }
        }
    }
}
