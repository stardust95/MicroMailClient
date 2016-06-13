import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Material 0.2
import Material.Extras 0.1
import Material.ListItems 0.1 as ListItem
import QtWebEngine 1.2


Item{

        id: mailView

//        property string mailWebView: mailWebView

        property string attachmentColor: Palette.colors["blue"]["200"]

        function loadHtml(content){
            mailWebView.loadHtml(content)
            console.log("in function loadHtml:"+content )
        }

        ColumnLayout{


            spacing: Units.dp(8)

            anchors.fill: parent
            anchors.margins: Units.dp(10)
            anchors.topMargin: 0

            RowLayout{
                id : mailViewToolBar

                height: Units.dp(60)

                spacing: 0

                anchors.top : parent.top

                anchors.right: parent.right

                Layout.fillWidth : true

                MyButton{
                    id : markAsUnreadButton
                    label : "Mark As Unread"
                    source: "/icons/mail_unread"
                    Layout.preferredWidth: Units.dp(label.length*8 + 60)
                }

                MyButton{
                    id : replyButton
                    label: "Reply"
                    source: "/icons/reply"
                    Layout.preferredWidth: Units.dp(label.length*8 + 60)

                }

                MyButton{
                    id : replyAllButton
                    label: "Reply All"
                    source: "/icons/reply_all"
                    Layout.preferredWidth: Units.dp(label.length*8 + 60)
                }

            }

            ProgressBar{
                Layout.fillWidth: true
                value: mailWebView.loadProgress/100.0
            }

            RowLayout{
                id: mailHeaderView

                height: mailViewToolBar.height

                spacing: Units.dp(10)

                Rectangle{
                    height: parent.height / 2
                    width: height
                    radius: height/2
                    color: theme.accentColor
                    Label{
                        anchors.centerIn: parent
                        text: mailHeaderSender.text.charAt(0)
                    }
                }

                ColumnLayout{
                    spacing: Units.dp(2)
                    Label  {
                        id : mailHeaderSender
                        text : mailListModel.getSender(selectedMailIndex)
                        font.bold: true
                        font.pixelSize: Units.dp(20)
                    }

                    Label {
                        id : mailHeaderDatetime
                        text : mailListModel.getDateTime(selectedMailIndex)
                        font.pixelSize: Units.dp(13)

                    }
                }

            }

            Label {
                text : mailListModel.getSubject(selectedMailIndex);
            }

            Label{
                text : mailListModel.getRecipients(selectedMailIndex);
            }

            RowLayout{

                id: attachmentsRow

                Layout.fillWidth: true

                height: Units.dp(50)

                Repeater{
                    id: attachmentsRepeater

                    model: mailListModel.getAttachments(selectedMailIndex)

                    delegate: Component{

                        MyButton{
                            source: "/icons/file"

                            backgroundColor: attachmentColor

                            height: parent.height

                            label: model.modelData

                            onClicked: {
                                mailListModel.downloadAttach(selectedMailIndex, index, ".");
//                                console.log( "selectedMailIndex" + selectedMailIndex )
//                                console.log(" index" + index)
                            }
                        }

                    }
                }
            }

            WebEngineView{

                id: mailWebView

                Layout.fillHeight: true
                Layout.fillWidth: true
                url: "https://www.baidu.com/"
//                            url: ""
                onLoadProgressChanged: console.log(mailWebView.loadProgress)

            }
        }
    }
