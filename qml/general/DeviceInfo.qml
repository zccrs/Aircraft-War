// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import QtMobility.systeminfo 1.2

Item{
    property string productName: deviceInfo.model
    DeviceInfo{
        id:deviceInfo
        //Component.onCompleted: {
            //console.log("手机型号:"+deviceInfo.model)
            //console.log("手机名字:"+deviceInfo.productName)
            //console.log("手机厂商:"+deviceInfo.manufacturer)
        //}
    }
}
