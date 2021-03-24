import QtQuick 2.15
import QtQuick.Controls 2.15
import view.type 1.0

Item {
    id: page
    objectName: "connectView"
    FontLoader { id: roboto; source: "fonts/roboto_black" }

    signal sendData(data: variant)

    function prepareData(host: string, port: string) {
        var data = {
            "host": host,
            "port": port,
            "view": ViewType.CONNECTION
        };
        sendData(data);
    }

    Connections {
        target: core
        function onUpdated() {
            busyIndicator.running = false
        }
    }

    Text {
        font.pointSize: 48
        text: "More Or Less ?"
        color: "#ffffff"
        font.family: roboto.name
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 50
    }
    Rectangle {
        width: parent.width / 2
        height: parent.height / 2
        color: "#bfffffff"
        radius: 30
        border.width: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        TextField {
            id: hostField
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            height: 50
            placeholderText: "Host (ex: 127.0.0.1)"
            text: "localhost"
            font.family: roboto.name
            font.pointSize: 18
            validator: RegularExpressionValidator { regularExpression: /(localhost)|(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3}/}
        }
        
        TextField {
            id: portField
            anchors.top: hostField.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            height: 50
            placeholderText: "Port (ex: 4242)"
            text: "4242"
            font.family: roboto.name
            font.pointSize: 18
            validator: RegularExpressionValidator { regularExpression: /^((6553[0-5])|(655[0-2][0-9])|(65[0-4][0-9]{2})|(6[0-4][0-9]{3})|([1-5][0-9]{4})|([0-5]{0,5})|([0-9]{1,4}))$/}
        }
        
        Rectangle {
            anchors.margins: 20
            anchors.top: portField.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            radius: 30
            color: "#BBFF0000"
            
            MouseArea {
                id: connectButton
                anchors.fill: parent
                onPressed: {
                    parent.color = "#99EE0000"
                }
                onReleased: {
                    parent.color = "#BBFF0000"
                }
                onClicked: {
                    busyIndicator.running = true;
                    prepareData(hostField.text, portField.text)
                }
                Text {
                    text: "Connect me!"
                    color: "white"
                    anchors.centerIn: parent
                    font.pointSize: 40
                    font.family: roboto.name
                }
            }
        }
    }
    Rectangle {
        id: busy
        color: "white"
        width: busyIndicator.width + 50
        height: busyIndicator.height + 50
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 15
        y: (busyIndicator.running) ? (parent.height - height + 15) : (parent.height)
        visible: busyIndicator.running
        Behavior on y {
            NumberAnimation {
                duration: 500
                easing.type: Easing.OutQuart
            }
        }

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: false
        }
    }
}