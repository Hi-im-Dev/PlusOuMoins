import QtQuick 2.15

Item {
    property var myText
    id: page
    objectName: "endView"
    FontLoader { id: roboto; source: "fonts/roboto_black" }

    signal reset()

    Text {
        id: statusText
        text: myText
        font.family: roboto.name
        font.pointSize: 30
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        y: 100
    }

    ListView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: statusText.bottom

        model: ListModel { id: model }
        delegate: Text {
            text: "[ " + date + " ] - [" + duration + "secs] - " + tries + " tries: " + status
            font.family: roboto.name
            font.pointSize: 20
            color: "white"
            anchors.fill: parent
        }

        Connections {
            target: core
            function onSendEndData(data: string) {
                var actual = JSON.parse(data);
                model.append({date: actual.startDate, duration: actual.duration, tries: actual.tries, status: actual.status})
            }
        }
    }
}
