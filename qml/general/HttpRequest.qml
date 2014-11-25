// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Item{
    id:root
    visible: false

    property string reData: ""
    signal postFinish
    signal error

    Timer{
        id:timer
        interval: 15000
        onTriggered:{
            error()
        }
    }

    function post(model, url, postdata)
    {
        var httpRequest = new XMLHttpRequest()

        httpRequest.open(model,url);

        httpRequest.setRequestHeader("Content-Type"," text/html; charset=utf-8");
        //httpRequest.setRequestHeader( "User-Agent","Mozilla/5.0 (iPad; U; CPU OS 4_2_1 like Mac OS X; zh-cn) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8C148 Safari/6533.18.5" )
        httpRequest.onreadystatechange=function(){
                    if(httpRequest.readyState===4){
                        if(httpRequest.status===200){
                            timer.stop()
                            reData = httpRequest.responseText
                            postFinish()
                        }
                    }
                }
        timer.start()
        if(model == "POST")
            httpRequest.send(postdata)
        else
            httpRequest.send(null)
    }
}
