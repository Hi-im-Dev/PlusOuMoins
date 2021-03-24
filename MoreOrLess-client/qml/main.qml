import QtQuick 2.15
import QtQml 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import view.type 1.0

// Window creation
Window {
    id: window
    width: 1280
    height: 720
    minimumWidth: 800
    minimumHeight: 600
    visible: true

    Component {
        id: profilePage
        Profile {}
    }

    Component {
        id: connectPage
        Connect {}
    }

    Component {
        id: gamePage
        Game {}
    }

    Component {
        id: endPageWin
        End {
            myText: "You won !"
        }
    }

    Component {
        id: endPageLose
        End {
            myText: "You lost !"
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#FDC830"
            }
            GradientStop {
                position: 1
                color: "#F37335"
            }
        }
    }
    
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: connectPage

        Connections {
            target: core
            function onPushView(viewType: int) {
                switch(viewType) {
                    case ViewType.PROFILE:
                        stackView.push(profilePage);
                        break;
                    case ViewType.GAME:
                        stackView.push(gamePage);
                        break;
                    case ViewType.END_WIN:
                        stackView.push(endPageWin);
                        break;
                    case ViewType.END_LOSE:
                        stackView.push(endPageLose);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    MessageDialog {
        id: dialog
        title: ""
        icon: StandardIcon.Warning
        text: ""
        standardButtons: StandardButton.Close
        onAccepted: {
            visible = false;
        }
        onRejected: {
            Qt.quit();
        }
        visible: false
    }


    Connections {
        target: core
        function onDisplayError(title: string, message: string, critical: bool) {
            dialog.title = title
            dialog.text = message
            dialog.standardButtons = critical == true ? StandardButton.Close : StandardButton.Ok
            dialog.visible = true
        }
    }

    Connections {
        target: core
        function onReset() {
            stackView.pop(null);
        }
    }
}
