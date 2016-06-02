import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem


Dialog {
    id: loginWindow

    title: "Account Login"

    visible: true

    width: Units.dp(350)

    height: column.implicitHeight + Units.dp(120)

    hasActions: false

    property alias signInButton : signInButton

    property alias cancelButton: cancelButton

    property alias addressText: addressTextField.text

    property alias passwordText: pwdField.text

    property var receiveProtocol: [ "IMAP", "POP3" ]

    ColumnLayout {
        id: column

        spacing: Units.dp(10)

        anchors {
            fill: parent
            topMargin: Units.dp(16)
            bottomMargin: Units.dp(16)
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Units.dp(8)
        }

        ListItem.Standard {
            action: Icon {
                anchors.centerIn: parent
                name: "communication/email"
            }

            content: TextField {
                anchors.centerIn: parent
                width: parent.width
                id: addressTextField
                text: "Alex Nelson"
            }
        }

        ListItem.Standard {
            action: Icon {
                anchors.centerIn: parent
                name: "communication/vpn_key"
            }

            content: TextField {
                id: pwdField
                anchors.centerIn: parent
                width: parent.width
                echoMode: TextInput.Password
                text: ""
            }
        }

        ListItem.Standard {
            action: Icon{
                anchors.centerIn: parent

            }

            content: RowLayout {
                anchors.centerIn: parent
                width: parent.width

                MenuField {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 0.35 * parent.width

                    model: receiveProtocol
                }

                TextField {
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: Units.dp(7)
                    Layout.fillWidth: true
//                    Layout.fillHeight: true
                    text: "New York"
                }

            }
        }

        ListItem.Standard {
            action: Icon {
                anchors.centerIn: parent
                name: "communication/email"
            }

            content: TextField {
                anchors.centerIn: parent
                width: parent.width

                placeholderText: "Email"
            }
        }

        RowLayout{


            anchors.horizontalCenter: parent.horizontalCenter

            spacing: Units.dp(20)

            Label{
                text: "Require SSL"
            }

            Switch{
                id: requireSSLSwitch
                anchors.verticalCenter: parent.verticalCenter
            }

        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Units.dp(8)
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: Units.dp(8)

            anchors {
                right: parent.right
                margins: Units.dp(16)
            }

            Button {
                id: cancelButton
                text: "Cancel"
                textColor: Theme.primaryColor
            }

            Button {
                id: signInButton
                text: "Done"
                textColor: Theme.primaryColor
            }
        }
    }


}
