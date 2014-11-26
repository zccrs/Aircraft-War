// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import com.star.widget 1.0

Item{
    id:rank_main

    property bool updateRanking: false

    height: parent.height
    width: parent.width
    opacity:0
    Behavior on opacity {
        NumberAnimation{duration: 300}
    }

    function keyAction(){}

    HttpRequest{
        id: get_network_rank
        onPostFinish: {
            var jsondata = JSON.parse(reData)
            if(jsondata){
                var count = jsondata.pager.count
                game_user_count.text = "总人数："+count
                for(var i=0; i<jsondata.ranks.length; ++i)
                {
                    var temp = {
                        nickname:jsondata.ranks[i].nickname,
                        score:jsondata.ranks[i].score,
                        avatar:jsondata.ranks[i].avatar,
                        user_model:jsondata.ranks[i].model,
                        gameRuningTime:jsondata.ranks[i].time
                    }
                    rank_model.append(temp)
                }
            }

            updateRanking = false
        }
        onError: {
            updateRanking = false
        }
    }

    function resetRank( isOntStart )//刷新排行榜
    {
        if(updateRanking)
            return
        updateRanking = true//避免重复刷新
        rank_model.clear()//先清空原来的排行榜
        rank_list.rank_page = 1
        var url="http://api.9smart.cn/ranks?clientid=5&page=1"
        get_network_rank.post("GET",url)
    }
    function addRank(page)//增加排行榜
    {
        if(updateRanking)
            return
        updateRanking = true//避免重复刷新

        if(rank_model.count>0 ){
            utility.console("增加排行榜")
            var url="http://api.9smart.cn/ranks?clientid=5&page="+String(page)
            get_network_rank.post("GET",url)
        }
    }

    Image{
        id:quit
        smooth: true
        opacity: 1-user_info_detail.opacity
        source: "qrc:/Image/Button_back.png"
        x:main.width/18
        scale: parent.height!=640?1:0.75
        y:main.quit_y

        MouseArea{
            anchors.fill: parent
            onClicked: {
                music.start_music("button")
                keyAction()
            }
        }
    }

    Image{
        id: rank01
        source: "qrc:/Image/Rank_01.png"
        sourceSize.width: parent.height!=640?410:410*0.75
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Image{
        id:rank02
        source: "qrc:/Image/Setting_02.png"
        sourceSize.width: parent.height!=640?410:410*0.75
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
    }

    Component{
        id: my_component
        Item{
            width: rank_list.width
            height: 60
            Text {
                id: number
                text: String(index+1)
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: main.height==854?24:18
            }
            MyImage{
                id:user_avatar

                width: 50
                height: 50
                smooth: true
                anchors.left: number.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                maskSource: "qrc:/Image/mask.bmp"
                source: avatar
            }
            Text{
                id:user_name
                anchors.left: user_avatar.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: user_score.left
                anchors.rightMargin: 10
                clip: true
                text: nickname
                font.pixelSize: main.height!=640?24:18
                font.family: localFont.name
            }

            Text{
                id: user_score
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                text:score
                font.pixelSize: main.height!=640?24:18
                font.family: localFont.name
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    music.start_music("button")
                    user_info_detail.open( user_model,gameRuningTime )
                }
            }
        }
    }

    ListModel{
        id:rank_model
    }

    Text {
        id: game_user_count
        anchors.bottom: rank01.bottom
        anchors.bottomMargin: 10
        anchors.right: rank_list.right
        color: "black"
        font.pixelSize: main.height!=640?26:22
        font.family: localFont.name
    }

    ListView{
        id:rank_list
        width: rank01.width
        opacity: 1-user_info_detail.opacity
        property int rank_page: 1
        anchors.top: rank01.bottom
        anchors.bottom: rank02.top
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        delegate: my_component

        model: rank_model

        onMovementEnded: {
            //utility.console("停止滑动")
            if( rank_list.contentY >= rank_list.contentHeight-rank_list.height )
                addRank( ++rank_page )
        }

        Component.onCompleted: resetRank( true )//刷新排行榜
    }

    Image{
        id:button_updataRank
        opacity: 1-user_info_detail.opacity
        source:resetmouse.pressed?"qrc:/Image/button_3_2.png":"qrc:/Image/button_3_1.png"
        scale:parent.height!=640?1:0.75
        smooth: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rank02.top
        anchors.topMargin: 20
        MouseArea{
            id:resetmouse
            anchors.fill: parent
            onClicked: {
                music.start_music("button")
                resetRank()//刷新排行榜
            }
        }
        Text{
            anchors.centerIn: parent
            font.family: localFont.name
            font.pixelSize: parent.height!=640?32:24
            text: "<b>刷新排行榜</b>";
            color: "#303030"
            //visible: main.height!=640?true:false
        }
    }
    Image{
        id: user_info_detail
        opacity: 0
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }
        source: "qrc:/Image/button_1.png"
        anchors.centerIn: parent
        sourceSize.width: parent.width-40
        clip:true
        function open(  text_phone_model, text_game_time ){
            phone_model.text = "手机型号："+text_phone_model
            game_time.text = "所用时间："+String(text_game_time)+"分钟"
            opacity = 1
        }

        Text{
            id: title
            font.family: localFont.name
            text:"详细信息"
            font.pixelSize: rank_main.height!=640?32:24
            anchors.horizontalCenter: parent.horizontalCenter
            y:20
        }
        Text{
            id: phone_model
            font.family: localFont.name
            text:"手机型号："
            font.pixelSize: rank_main.height!=640?32:22
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: title.bottom
            anchors.topMargin: 30
        }
        Text{
            id: game_time
            font.family: localFont.name
            text:"所用时间："
            font.pixelSize: rank_main.height!=640?32:22
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: phone_model.bottom
            anchors.topMargin: 10
        }
        Image{
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
                    user_info_detail.opacity = 0
                }
            }
        }
    }
}
