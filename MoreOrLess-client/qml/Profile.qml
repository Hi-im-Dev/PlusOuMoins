import QtQuick 2.15
import QtQuick.Controls 2.15
import view.type 1.0

Item {
    id: page
    objectName: "profileView"
    FontLoader { id: roboto; source: "fonts/roboto_black" }

    signal sendData(data: variant)

    function prepareData(username: string, bot: bool) {
        var data = {
            "username": username,
            "bot": bot,
            "view": ViewType.PROFILE
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
        text: "Choose your settings !"
        color: "#ffffff"
        font.family: roboto.name
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 50
    }

    Rectangle {
        width: parent.width / 2 + 150
        height: parent.height / 2
        color: "#bfffffff"
        radius: 30
        border.width: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    
        TextField {
            id: nameField
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            height: 50
            placeholderText: "Name (empty for anonymous)"
            font.family: roboto.name
            font.pointSize: 18
        }
        
        CheckBox {
            id: autoCheck
            anchors.top: nameField.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            height: 50
            text: "Auto (Bot) (Will deactivate name field)"
            font.family: roboto.name
            font.pointSize: 18
        }

        Rectangle {
            anchors.margins: 20
            anchors.top: autoCheck.bottom
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
                    prepareData(nameField.text, autoCheck.checkedState)
                    busyIndicator.running = true;
                }
                Text {
                    text: "Start the game!"
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
