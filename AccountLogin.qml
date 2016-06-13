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

    property alias selectedProtocol: protocolMenu.selectedText

    property alias sendHost: sendHostText.text

    property alias receiveHost: receiveHostText.text

    property alias requireSSL: requireSSLSwitch.checked

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
                text: "375670450@qq.com"
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
                text: "sftkpahwbroabhjg"
            }
        }



        ListItem.Standard {

            Layout.preferredHeight: Units.dp(30)

            content: RowLayout {
                anchors.centerIn: parent
                width: parent.width

                Label {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 0.3 * parent.width
                    text: "SMTP"
                }

                TextField {
                    id: sendHostText
                    anchors.bottom: parent.bottom
                    Layout.fillWidth: true
                    placeholderText: "smtp.qq.com"
                    text: "smtp.qq.com"
                }

            }
        }

        ListItem.Standard {

            Layout.preferredHeight: Units.dp(30)

            content: RowLayout {
                anchors.centerIn: parent
                width: parent.width

                MenuField {
                    id: protocolMenu
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: 0.3 * parent.width
                    model: receiveProtocol
                }

                TextField {
                    id: receiveHostText
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: Units.dp(15)
                    Layout.fillWidth: true
                    placeholderText: "imap.qq.com"
                    text: "imap.qq.com"
                }

            }
        }

        RowLayout{
//            anchors.horizontalCenter: parent.horizontalCenter

            spacing: Units.dp(30)

            Label{
                text: "Require SSL"
            }

            Switch{
                id: requireSSLSwitch
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    requireSSL = !requireSSL
                    console.log("require ssl = " + requireSSL)
                }
                checked: true
            }

        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Units.dp(8)
        }

        RowLayout {

            Layout.alignment: Qt.AlignHCenter
            spacing: Units.dp(8)

            Button {
                id: cancelButton
                text: "Cancel"
                textColor: Theme.primaryColor
                onClicked: close()
            }

            Button {
                id: signInButton
                text: "Login"
                textColor: Theme.primaryColor
            }
        }
    }


}
