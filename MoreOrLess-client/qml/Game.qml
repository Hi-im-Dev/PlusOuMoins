import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import view.type 1.0

Item {
    id: page
    objectName: "gameView"
    FontLoader { id: roboto; source: "fonts/roboto_black" }

    signal sendData(data: variant)

    function prepareData(guess: int) {
        var data = {
            "guess": parseInt(guess),
            "view": ViewType.GAME
        };
        sendData(data);
    }

    Connections {
        target: core
        function onValueChanged(value: string) {
            plusOrLess.text = value
            serverRect.color = value === "+" ? "red" : "blue"
            anim2.running = true
        }
    }


    Rectangle {
        id: serverRect
        width: 200
        height: 200
        radius: 10
        y: 75
        NumberAnimation {
            id: anim1
            target: serverRect
            property: "y"
            to: -serverRect.height - 100
            duration: 500
            onFinished: {
                prepareData(guessField.text)
                guessField.text = ""
            }
            easing.type: Easing.OutQuart
        }
        NumberAnimation {
            id: anim2
            target: serverRect
            property: "y"
            to: 75
            duration: 500
            easing.type: Easing.OutQuart
        }
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            anchors.centerIn: parent
            id: plusOrLess
            font.family: roboto.name
            font.pointSize: 40
            color: "white"
            text: "?"
        }
    }

    Rectangle {
        id: userRect
        width: parent.width - 200
        height: 200
        radius: 10
        y: parent.height - height - 75
        anchors.horizontalCenter: parent.horizontalCenter

        TextField {
            id: guessField
            anchors.fill: parent
            anchors.margins: 20
            placeholderText: "Type your guess and press Enter !"
            font.family: roboto.name
            font.pointSize: 26
            validator: IntValidator {}
            horizontalAlignment: TextInput.AlignHCenter
            background: Rectangle {
                color: "white"
            }
            onAccepted: {
                anim1.running = true
            }
        }
    }
}
