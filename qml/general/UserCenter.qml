// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import QtWebKit 1.0
import com.star.widget 1.0

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
                var url = "http://api.9smart.cn/rank/"+String(uid)+"?clientid=5"
                post_ranking.post("GET",url)
            }
        }
    }

    function updataData()
    {
        var uid=mysettings.getValue("user_uid","")
        if(uid!=""){
            user_avatar .source = mysettings.getValue("user_avatar","")
            user_nickname.text = "用户名："+mysettings.getValue("user_nickname","")

            var url = "http://api.9smart.cn/rank/"+String(uid)+"?clientid=5"
            post_ranking.post("GET",url)
        }else{
            user_nickname.text = "用户名：未登录"
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

    MyImage{
        id:user_avatar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: setting01.bottom
        anchors.topMargin: 10
        width: parent.width >480 ?80:100
        smooth: true
        source: mysettings.getValue("user_avatar","")
        maskSource: "qrc:/Image/mask.bmp"

        MouseArea{
            anchors.fill: parent
            onClicked: {
                if(user_avatar.maskSource=="")
                    user_avatar.maskSource="qrc:/Image/mask.bmp"
                else
                    user_avatar.maskSource=""
            }
        }
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
            var jsondata = JSON.parse(reData)
            if(jsondata&&jsondata.error==0){
                user_phone_model.text = "玩家设备："+String(jsondata.rank.model)
                user_ranking.text = "网络排行："+String(jsondata.rank.top)
                user_score.text = "最新得分："+String(jsondata.rank.score)
                mysettings.setValue("user_score",parseInt(jsondata.rank.score))
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
                webLogin.url = "http://www.9smart.cn/member/login"
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
        preferredHeight: settings_main.height
        preferredWidth:settings_main.width
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }

        onLoadFinished: {
            utility.console(title)

            var jsondata=JSON.parse(title)
            if( jsondata ){
                //utility.console("登录成功")

                opacity = 0//登陆成功，返回游戏
                mysettings.setValue("user_uid",jsondata.uid)
                mysettings.setValue("user_nickname",jsondata.nickname)
                mysettings.setValue("user_avatar",jsondata.avatar2)
                mysettings.setValue("logintype", jsondata.logintype)
                mysettings.setValue("accesstoken", jsondata.accesstoken)
                mysettings.setValue("user_score",0)//登录完成先将新用户的得分初始化为0

                utility.console("user_uid："+mysettings.getValue("user_uid",""))
                utility.console("user_nickname："+mysettings.getValue("user_nickname",""))
                utility.console("user_avatar："+mysettings.getValue("user_avatar",""))

                updataData()//更新用户信息
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
                if(webLogin.url!=""){
                    webLogin.opacity=0
                    webLogin.url = ""
                }
            }
        }
    }
}
