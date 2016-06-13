import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem
import QtWebEngine 1.2

Item {

    id:newMailRectangle

    //@Zach's param
    property bool isTextBold: false
    property bool isTextItalic: false
    property bool isTextUnderline: false
    property bool isTextStrikeout: false
    property bool isTextSubscript: false
    property bool isTextSuperscript: false
    property string textColor: "black"

    //@Zach's param end

//    width:parent.width
//          - menuSidebar.width
          //-(menuSidebar.expanded?menuSidebar.width:thinMenuSidebar.width)
//          - mailListColumn.width
//    height:parent.height

    anchors.fill: parent


    ColumnLayout{
//        x:15
        id:newMailRectangleColumn
        anchors.fill: parent
        anchors.margins: Units.dp(10)
        spacing: Units.dp(8)



        Rectangle{
            id: editTextButtonRectangle
            anchors.left: parent.left
            width: 0.95*parent.width
            height: Units.dp(45)

            color:"#E0E0E0"

            RowLayout{
                anchors.fill: parent

                Button{

                    id: textBoldButton
                    width: Units.dp(45)
//                    anchors.left: parent.left
                    Label{

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"B"
                        font.bold: true
                        font.pixelSize: 30
                    }

                    onClicked: {
                        isTextBold=isTextBold?false:true
                        var start = newMailTextEdit.selectionStart
                        var end  = newMailTextEdit.selectionEnd
                        var text = newMailTextEdit.selectedText
                        newMailTextEdit.remove(start, end)
                        newMailTextEdit.insert(start, "<strong>" + text +"</strong>")

                        console.log(newMailTextEdit.text)
                    }



                    backgroundColor: isTextBold?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textItalicButton
                    width: Units.dp(45)
//                    anchors.left: textBoldButton.right
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"I"
                        font.italic: true
                        font.pixelSize: 30
                    }
                    onClicked: isTextItalic=isTextItalic?false:true
                    backgroundColor: isTextItalic?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textUnderlineButton
                    width: Units.dp(45)
//                    anchors.left: textItalicButton.right
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"U"
                        font.underline: true
                        font.pixelSize: 30
                    }
                    onClicked: isTextUnderline=isTextUnderline?false:true
                    backgroundColor: isTextUnderline?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textStrikeoutButton
                    width: Units.dp(45)
//                    anchors.left: textUnderlineButton.right
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"ab"
                        font.strikeout: true
                        font.pixelSize: 30
                    }
                    onClicked: isTextStrikeout=isTextStrikeout?false:true
                    backgroundColor: isTextStrikeout?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                MenuField{
                    id:fontMenuField

//                    anchors.left: textStrikeoutButton.right
                    height: Units.dp(40)
                    Layout.alignment: Qt.AlignLeft
                    model:["font1", "font2", "font3", "font4"]









                }



                MenuField{
                    id:fontSizeMenuField
//                    anchors.left: fontMenuField.right
                    height: Units.dp(40)
                    model:["1","2","3","4","5"]
                }

                Button{
                    id: textSubscriptButton
                    width: Units.dp(45)
//                    anchors.left: fontSizeMenuField.right
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"x<sub>2</sub>"
                        textFormat: Text.RichText
                        font.pixelSize: 30
                    }
                    onClicked: isTextSubscript=isTextSubscript?false:true
                    backgroundColor: isTextSubscript?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textSuperscriptButton
                    width: Units.dp(45)
//                    anchors.left: textSubscriptButton.right
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"x<sup>2</sup>"
                        textFormat: Text.RichText
                        font.pixelSize: 30
                    }
                    onClicked: isTextSuperscript=isTextSuperscript?false:true
                    backgroundColor: isTextSuperscript?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textColorPickButton
                    width: Units.dp(45)


                    Layout.fillWidth: true

//                    anchors.left: textSuperscriptButton.right
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"<u>A</u>"
                        textFormat: Text.RichText
                        font.pixelSize: 30
                    }
                    onClicked: colorPicker.open()
                    backgroundColor: editTextButtonRectangle.color

                }



            }


        }  //end of editTextButtonRectangle

        ListItem.Standard{
            id:newMailSender
            content:RowLayout{
                Label{
                    id: newMailFromLabel
                    Layout.preferredWidth: Units.dp(35)
                    text:"From: "
                }

                MenuField{
                    model:["sender1", "longonglonglongsender2", "sender3"]
                    Layout.preferredWidth: 0.9*newMailSender.width-newMailFromLabel.width
                }
            }
        }

        ListItem.Standard{
            id: newMailReceiver
            content:RowLayout{
                width: parent.width* 0.95

                Label{
                    id: toReceiverLabel
                    text:"To: "
                    Layout.alignment:Qt.AlignLeft
                    Layout.preferredWidth:  Units.dp(20)
                }

                TextField{
                    Layout.preferredWidth: parent.width-toReceiverLabel.width-1.1*ccBCCButton.width
                }

                MyButton{
                    id:ccBCCButton
                    text:"CC & BCC"
                    anchors.right: parent.right
                    onClicked: {
                        console.log( "width = " + newMailTextEdit.width )
                        console.log( "height = " + newMailTextEdit.height )
                    }

//                        width:Units.dp(80)
                }
            }
        }//end of newMailReceiver

        ListItem.Standard{
            id:newMailSubject
            content: TextField{
                anchors.left: parent.lest
                width: parent.width* 0.95
                placeholderText: "Subject"
            }
        }

        TextEdit {

            id: newMailTextEdit

            anchors.margins: Units.dp(10)
            Layout.fillWidth: true
            Layout.fillHeight: true

            selectByMouse:true
            font.pixelSize: 16
            font.family: "Microfsoft YaHei"

            color: "black"  // should be the picked color

            Keys.onReleased: {

            }

            focus: true
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.RichText
         }

    }  //end of columnlayout

    Dialog {
            id: colorPicker
            hasActions: true
//            visible: false
            title: "Pick color"
            height: Units.dp(100)
            width : Units.dp(100)
            positiveButtonText: "Done"
            z:2

            Grid {
                columns: 7
                spacing: Units.dp(8)

                Repeater {
                    model: [
                        "red", "pink", "purple", "deepPurple", "indigo",
                        "blue", "lightBlue", "cyan", "teal", "green",
                        "lightGreen", "lime", "yellow", "amber", "orange",
                        "deepOrange", "grey", "blueGrey", "brown", "black",
                        "white"
                    ]

                    Rectangle {
                        width: Units.dp(30)
                        height: Units.dp(30)
                        radius: Units.dp(2)
                        color: Palette.colors[modelData]["500"]
                    }
                }
            }

            onOpened: {
                console.log("color picker dialog opened")
            }

            onRejected: {
                // TODO set default colors again but we currently don't know what that is
            }
        }//end of colorPickerDialog

}

