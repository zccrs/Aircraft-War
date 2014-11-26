// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import QtMobility.systeminfo 1.1

Item{
    property string productName: deviceInfo.model
    DeviceInfo{
        id:deviceInfo
    }
}
