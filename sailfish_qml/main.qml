// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow{
    cover: ""

    initialPage: MainPage{}

    Component.onCompleted: {
        if(_roundedCorners){
            _roundedCorners = false;
        }
    }
}
