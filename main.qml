import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    width: 800
    height: 600

    Button {
        text: "button"
        anchors.top: parent.top
        anchors.right: parent.right
        onClicked: console.log("pressed")
    }

    TextField {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}
