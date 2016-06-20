import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import Material 0.2
import Material.ListItems 0.1 as ListItem
import QtWebEngine 1.2

Item {

    id:newMailRectangle

    //@Zach's param
    property bool isTextBold: false
    property bool textReadyToBold: false
    property bool textReadyNotBold: false
    property bool isTextItalic: false
    property bool textReadyToItalic: false
    property bool textReadyNotItalic: false
    property bool isTextUnderline: false
    property bool textReadyToUnderline: false
    property bool textReadyNotUnderline: false
    property bool isTextStrikeout: false
    property bool textReadyToStrikeout: false
    property bool textReadyNotStrikeout: false
    property bool isTextSubscript: false
    property bool textReadyToSub: false
    property bool textReadyNotSub: false
    property bool isTextSuperscript: false
    property bool textReadyToSup: false
    property bool textReadyNotSup: false

    property string textColor: "black"
    property string selectedPlainText: ""
    property string selectedFormattedText: ""
    property var textFontFamily: [
        "Arial", "Calibri", "Courier New", "Microsoft YaHei", "宋体"]
    property var textFontSize: [
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30]
    property var attachmentList : []


    //@Zach's param end

    anchors.fill: parent

    ColumnLayout{
        id:newMailRectangleColumn
        anchors.fill: parent
        anchors.margins: Units.dp(10)
        anchors.topMargin: 0
        spacing: Units.dp(8)

        RowLayout{
            id : mailViewToolBar

            height: Units.dp(60)

            spacing: 0

            anchors.top : parent.top

            anchors.right: parent.right

            Layout.fillWidth : true

            MyButton{
                id : sendMailButton
                label : "Send"
                source: "/icons/send"
                Layout.preferredWidth: Units.dp(label.length*8 + 60)
                onClicked: {
                    console.log(typeof(attachmentList), attachmentList)
                    mailListModel.sendMail(recipientTextField.text, subjectTextField.text, newMailTextEdit.getFormattedText(0, newMailTextEdit.text.length), attachmentList)
                    snackbar.open("Mail Send Successfully")
                }

            }

            MyButton{
                id : clearButton
                label: "Reply"
                source: "/icons/clear"
                Layout.preferredWidth: Units.dp(label.length*8 + 60)

            }

            MyButton{
                id : replyAllButton
                label: "Reply All"
                source: "/icons/reply_all"
                Layout.preferredWidth: Units.dp(label.length*8 + 60)
            }

        }


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
                    Layout.preferredWidth: Units.dp(45)
                    Label{

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"B"
                        font.bold: true
                        font.pixelSize: 30
                    }

                    onClicked: {
                        var start = newMailTextEdit.selectionStart
                        var end  = newMailTextEdit.selectionEnd
                        selectedFormattedText = newMailTextEdit.getFormattedText(start, end)
                        selectedPlainText=newMailTextEdit.selectedText
                        var hasSelected=(start==end)? false: true


                        if(hasSelected){
                            if(selectedFormattedText.indexOf("font-weight:600;")!=-1)
                            {
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, selectedPlainText)
                                isTextBold=false
                                newMailTextEdit.select(start, end)
                            }
                            else{
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "<span style=\" font-weight:600;font-style:inherit;text-decoration:inherit\">" + selectedPlainText +"</span>")
                                isTextBold=true
                                newMailTextEdit.select(start, end)
                            }
                        }
                        else{
                            if(isTextBold){
                                isTextBold=false
                                textReadyToBold=false
                                textReadyNotBold=true
                            }
                            else{
                                isTextBold=true
                                textReadyToBold=true
                                textReadyNotBold=false
							}
                        }
                    }

                    backgroundColor: isTextBold?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textItalicButton
                    Layout.preferredWidth: Units.dp(45)
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"I"
                        font.italic: true
                        font.pixelSize: 30
                    }
                    onClicked: {
                        var start = newMailTextEdit.selectionStart
                        var end  = newMailTextEdit.selectionEnd
                        selectedFormattedText = newMailTextEdit.getFormattedText(start, end)
                        selectedPlainText=newMailTextEdit.selectedText
                        var hasSelected=(start==end)? false: true


                        if(hasSelected){
                            if(selectedFormattedText.indexOf("font-style:italic;")!=-1)
                            {
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, selectedPlainText)
                                isTextItalic=false
								newMailTextEdit.select(start, end)
                            }
                            else{
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "<i>" + selectedPlainText +"</i>")
                                isTextItalic=true
								newMailTextEdit.select(start, end)
                            }
                        }
                        else{
                            if(isTextItalic){
                                isTextItalic=false
                                textReadyToItalic=false
                                textReadyNotItalic=true
                            }
                            else{
                                isTextItalic=true
                                textReadyToItalic=true
                                textReadyNotItalic=false
                            }
                        }
                    }
                    backgroundColor: isTextItalic?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textUnderlineButton
                    Layout.preferredWidth: Units.dp(45)
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"U"
                        font.underline: true
                        font.pixelSize: 30
                    }
					
                    onClicked: {
                        var start = newMailTextEdit.selectionStart
                        var end  = newMailTextEdit.selectionEnd
                        selectedFormattedText = newMailTextEdit.getFormattedText(start, end)
                        selectedPlainText=newMailTextEdit.selectedText
                        var hasSelected=(start==end)? false: true

                        if(hasSelected){
                            if(selectedFormattedText.indexOf("text-decoration: underline")!=-1)
                            {
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "</span>" + selectedPlainText)
                                isTextUnderline=false
                                newMailTextEdit.select(start, end)
                            }
                            else{
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "<span style=\"text-decoration: underline; font-style: inherit; font-weight:inherit\">" + selectedPlainText +"</span>")
                                isTextUnderline=true
                                newMailTextEdit.select(start, end)
                            }
                        }
                        else{
                            if(isTextUnderline){
                                isTextUnderline=false
                                textReadyToUnderline=false
                                textReadyNotUnderline=true
                            }
                            else{
                                isTextUnderline=true
                                textReadyToUnderline=true
                                textReadyNotUnderline=false
							}
                        }
                    }
                    backgroundColor: isTextUnderline?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                }

                Button{
                    id: textStrikeoutButton
                    Layout.preferredWidth: Units.dp(45)
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"ab"
                        font.strikeout: true
                        font.pixelSize: 30
                    }
                    backgroundColor: isTextStrikeout?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
					
					onClicked: {
                        var start = newMailTextEdit.selectionStart
                        var end  = newMailTextEdit.selectionEnd
                        selectedFormattedText = newMailTextEdit.getFormattedText(start, end)
                        selectedPlainText=newMailTextEdit.selectedText
                        var hasSelected=(start==end)? false: true


                        if(hasSelected){
                            if(selectedFormattedText.indexOf("text-decoration: line-through")!=-1)
                            {
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "</span>" + selectedPlainText)
                                isTextStrikeout=false
                                newMailTextEdit.select(start, end)
                            }
                            else{
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "<span style=\"font-weight:inherit;font-style:inherit;text-decoration:line-through;\">" + selectedPlainText +"</span>")
                                isTextStrikeout=true
                                newMailTextEdit.select(start, end)
                            }
                        }
                        else{
                            if(isTextStrikeout){
                                isTextStrikeout=false
                                textReadyToStrikeout=false
                                textReadyNotStrikeout=true
                            }
                            else{
                                isTextStrikeout=true
                                textReadyToStrikeout=true
                                textReadyNotStrikeout=false
							}
                        }
                    }
                }

                MenuField{
                    id:fontMenuField

                    model:textFontFamily
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: Units.dp(240)
                    selectedIndex: 3

                    Layout.fillWidth: true

//                    onSelectedIndexChanged: {
//                        var start = newMailTextEdit.selectionStart
//                        var end  = newMailTextEdit.selectionEnd
//                        selectedFormattedText = newMailTextEdit.getFormattedText(start, end)
//                        selectedPlainText=newMailTextEdit.selectedText
//                        var hasSelected=(start==end)? false: true

//                        if(hasSelected){
//                            newMailTextEdit.remove(start, end)
//                            newMailTextEdit.insert(start, "<span style=\"font-weight:inherit;font-style:inherit;text-decoration:inhert;font-size:"+(1+selectedIndex).toString()+"px;\">" + selectedPlainText +"</span>")
//                            newMailTextEdit.select(start, end)
//                        }
//                        console.log(newMailTextEdit.getFormattedText(start, end))
//                    }
                }

                MenuField{
                    id:fontSizeMenuField
                    Layout.preferredWidth: Units.dp(60)
                    model:textFontSize
                    selectedIndex: 15
                }

                Button{
                    id: textSubscriptButton
                    Layout.preferredWidth: Units.dp(45)
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"x<sub>2</sub>"
                        textFormat: Text.RichText
                        font.pixelSize: 25
                    }
                    backgroundColor: isTextSubscript?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
					onClicked: {
                        var start = newMailTextEdit.selectionStart
                        var end  = newMailTextEdit.selectionEnd
                        selectedFormattedText = newMailTextEdit.getFormattedText(start, end)
                        selectedPlainText=newMailTextEdit.selectedText
                        var hasSelected=(start==end)? false: true


                        if(hasSelected){
                            if(selectedFormattedText.indexOf("vertical-align:sub")!=-1)
                            {
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, selectedPlainText)
                                isTextSubscript=false
                                newMailTextEdit.select(start, end)
                            }
                            else{
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "<span style=\" font-weight:inherit;font-style:inherit;text-decoration:inherit;vertical-align:sub\">" + selectedPlainText +"</span>")
                                isTextSubscript=true
                                newMailTextEdit.select(start, end)
                            }
                        }
                        else{
                            if(isTextSubscript){
                                isTextSubscript=false
                                textReadyToSub=false
                                textReadyNotSub=true
                            }
                            else{
                                isTextSubscript=true
                                textReadyToSub=true
                                textReadyNotSub=false
							}
                        }
                    }
                }

                Button{
                    id: textSuperscriptButton
                    Layout.preferredWidth: Units.dp(45)
//                    anchors.left: textSubscriptButton.right
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"x<sup>2</sup>"
                        textFormat: Text.RichText
                        font.pixelSize: 25
                    }
					backgroundColor: isTextSuperscript?(Qt.darker(editTextButtonRectangle.color, 1.2)):editTextButtonRectangle.color
                    onClicked: {
                        var start = newMailTextEdit.selectionStart
                        var end  = newMailTextEdit.selectionEnd
                        selectedFormattedText = newMailTextEdit.getFormattedText(start, end)
                        selectedPlainText=newMailTextEdit.selectedText
                        var hasSelected=(start==end)? false: true


                        if(hasSelected){
                            if(selectedFormattedText.indexOf("vertical-align:super")!=-1)
                            {
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, selectedPlainText)
                                isTextSuperscript=false
                                newMailTextEdit.select(start, end)
                            }
                            else{
                                newMailTextEdit.remove(start, end)
                                newMailTextEdit.insert(start, "<span style=\" font-weight:inherit;font-style:inherit;text-decoration:inherit;vertical-align:super\">" + selectedPlainText +"</span>")
                                isTextSuperscript=true
                                newMailTextEdit.select(start, end)
                            }
                        }
                        else{
                            if(isTextSuperscript){
                                isTextSuperscript=false
                                textReadyToSup=false
                                textReadyNotSup=true
                            }
                            else{
                                isTextSuperscript=true
                                textReadyToSup=true
                                textReadyNotSup=false
							}
                        }
                    }
                }

                Button{
                    id: textColorPickButton
                    Layout.preferredWidth: Units.dp(45)

                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"<u>A</u>"
                        textFormat: Text.RichText
                        font.pixelSize: 25
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
                    model:[ user ]
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
                    id: recipientTextField
                    Layout.preferredWidth: parent.width-toReceiverLabel.width-1.1*ccBCCButton.width
                }

                MyButton{
                    id:ccBCCButton
                    text:"CC & BCC"
                    anchors.right: parent.right
                    onClicked: {
                        ccMailReceiver.visible= !ccMailReceiver.visible
                        bccMailReceiver.visible=!bccMailReceiver.visible
                    }
                }
            }
        }//end of newMailReceiver

        ListItem.Standard{
            id:ccMailReceiver
            visible:false
            content:RowLayout{
                width: parent.width* 0.95

                Label{
                    id: ccReceiverLabel
                    text:"CC: "
                    Layout.alignment:Qt.AlignLeft
                    Layout.preferredWidth:  Units.dp(20)
                }

                TextField{
                    id: ccTextField
                    Layout.preferredWidth: parent.width-ccReceiverLabel.width
                }
            }
        }

        ListItem.Standard{
            id:bccMailReceiver
            visible:false
            content:RowLayout{
                width: parent.width* 0.95

                Label{
                    id: bccReceiverLabel
                    text:"BCC: "
                    Layout.alignment:Qt.AlignLeft
                    Layout.preferredWidth:  Units.dp(28)
                }

                TextField{
                    id: bccTextField
                    Layout.preferredWidth: parent.width-bccReceiverLabel.width
                }
            }
        }

        ListItem.Standard{
            id:newMailSubject
            content: RowLayout{
                width:parent.width*0.95
                TextField{
                    id: subjectTextField
                    anchors.left: parent.lest
                    Layout.preferredWidth: parent.width-attachmentButton.width*1.1
                    placeholderText: "Subject"
                }

                MyButton{
                    id:attachmentButton
                    text:"attachment"
                    anchors.right: parent.right
                    onClicked: {
                        attaPickerDialog.open()
                    }
                }
            }
        }

        RowLayout{
            id: attachmentRowLayout
            Layout.fillWidth: true
            height: Units.dp(50)
            visible: attachmentList.length > 0

            Repeater{
                id: attachmentRepeater
                model: attachmentList
                delegate:Component{
                    MyButton{
                        source: "/icons/file"
                        height: 0.95*parent.height
                        label: modelData.substr(modelData.lastIndexOf('/')+1)
                    }
                }
            }
        }

        TextEdit {
            id: newMailTextEdit

            anchors.margins: Units.dp(10)
            cursorVisible: true //debug
            Layout.fillHeight: true
            Layout.fillWidth: true

            font.pixelSize: fontSizeMenuField.selectedIndex + 1
            font.family: fontMenuField.selectedText

            color: textColor  // should be the picked color

            Keys.onReleased: {
                if(isTextBold && textReadyToBold)
                {
                    var currentCursor=newMailTextEdit.cursorPosition
                    var ch=newMailTextEdit.getText(currentCursor-1,currentCursor)
                    newMailTextEdit.remove(currentCursor-1, currentCursor)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<strong>"+ch+"</strong>")
                    textReadyToBold=false
                }
                else if(!isTextBold && textReadyNotBold)
                {
                    var currentCursor2=newMailTextEdit.cursorPosition
                    var ch2=newMailTextEdit.getText(currentCursor2-1,currentCursor2)
                    newMailTextEdit.remove(currentCursor2-1, currentCursor2)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"font-weight:0\">"+ch2+"</span>")
                    textReadyNotBold=false
                }

                if(isTextItalic && textReadyToItalic)
                {
                    var currentCursor3=newMailTextEdit.cursorPosition
                    var ch3=newMailTextEdit.getText(currentCursor3-1,currentCursor3)
                    newMailTextEdit.remove(currentCursor3-1, currentCursor3)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"font-weight:inherit;font-style:italic;text-decoration:inherit;\">"+ch3+"</span>")
                    textReadyToItalic=false
                }
                else if(!isTextItalic && textReadyNotItalic)
                {
                    var currentCursor4=newMailTextEdit.cursorPosition
                    var ch4=newMailTextEdit.getText(currentCursor4-1,currentCursor4)
                    newMailTextEdit.remove(currentCursor4-1, currentCursor4)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"font-style:normal\">"+ch4+"</span>")
                    textReadyNotItalic=false
                }

                if(isTextUnderline && textReadyToUnderline)
                {
                    var currentCursor5=newMailTextEdit.cursorPosition
                    var ch5=newMailTextEdit.getText(currentCursor5-1,currentCursor5)
                    newMailTextEdit.remove(currentCursor5-1, currentCursor5)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"font-weight:inherit;font-style:inherit;text-decoration:underline;\">"+ch5+"</span>")
                    textReadyToUnderline=false
                }
                else if(!isTextUnderline && textReadyNotUnderline)
                {
                    var currentCursor6=newMailTextEdit.cursorPosition
                    var ch6=newMailTextEdit.getText(currentCursor6-1,currentCursor6)
                    newMailTextEdit.remove(currentCursor6-1, currentCursor6)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"text-decoration:normal\">"+ch6+"</span>")
                    textReadyNotUnderline=false
                }

                if(isTextStrikeout && textReadyToStrikeout)
                {
                    var currentCursor7=newMailTextEdit.cursorPosition
                    var ch7=newMailTextEdit.getText(currentCursor7-1,currentCursor7)
                    newMailTextEdit.remove(currentCursor7-1, currentCursor7)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"font-weight:inherit;font-style:inherit;text-decoration:line-through;\">"+ch7+"</span>")
                    textReadyToStrikeout=false
                }
                else if(!isTextStrikeout && textReadyNotStrikeout)
                {
                    var currentCursor8=newMailTextEdit.cursorPosition
                    var ch8=newMailTextEdit.getText(currentCursor8-1,currentCursor8)
                    newMailTextEdit.remove(currentCursor8-1, currentCursor8)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"text-decoration:normal\">"+ch8+"</span>")
                    textReadyNotStrikeout=false
                }

                if(isTextSubscript && textReadyToSub)
                {
                    var currentCursor9=newMailTextEdit.cursorPosition
                    var ch9=newMailTextEdit.getText(currentCursor9-1,currentCursor9)
                    newMailTextEdit.remove(currentCursor9-1, currentCursor9)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"font-weight:inherit;font-style:inherit;text-decoration:inherit;vertical-align:sub\">"+ch9+"</span>")
                    textReadyToSub=false
                }
                else if(!isTextSubscript && textReadyNotSub)
                {
                    var currentCursorA=newMailTextEdit.cursorPosition
                    var chA=newMailTextEdit.getText(currentCursorA-1,currentCursorA)
                    newMailTextEdit.remove(currentCursorA-1, currentCursorA)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"vertical-align:normal\">"+chA+"</span>")
                    textReadyNotSub=false
                }

                if(isTextSuperscript && textReadyToSup)
                {
                    var currentCursorB=newMailTextEdit.cursorPosition
                    var chB=newMailTextEdit.getText(currentCursorB-1,currentCursorB)
                    newMailTextEdit.remove(currentCursorB-1, currentCursorB)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"font-weight:inherit;font-style:inherit;text-decoration:inherit;vertical-align:sub\">"+chB+"</span>")
                    textReadyToSup=false
                }
                else if(!isTextSuperscript && textReadyNotSup)
                {
                    var currentCursorC=newMailTextEdit.cursorPosition
                    var chC=newMailTextEdit.getText(currentCursorC-1,currentCursorC)
                    newMailTextEdit.remove(currentCursorC-1, currentCursorC)
                    newMailTextEdit.insert(newMailTextEdit.cursorPosition, "<span style=\"vertical-align:normal\">"+chC+"</span>")
                    textReadyNotSup=false
                }
            }

//            property string tmpFormattedString

//            onCursorPositionChanged: {
//                tmpFormattedString=getFormattedText(cursorPosition-1, cursorPosition)

//                if(tmpFormattedString.indexOf("font-weight:600")!=-1)
//                    isTextBold=true
//                else
//                    isTextBold=false
//                if(tmpFormattedString.indexOf("font-style:italic")!=-1)
//                    isTextItalic=true
//                else
//                    isTextItalic=false
//                if(tmpFormattedString.indexOf("text-decoration: underline")!=-1)
//                    isTextUnderline=true
//                else
//                    isTextUnderline=false
//                if(tmpFormattedString.indexOf("text-decoration: line-through")!=-1)
//                    isTextStrikeout=true
//                else
//                    isTextStrikeout=false
//                if(tmpFormattedString.indexOf("vertical-align:sub")!=-1)
//                    isTextSubscript=true
//                else
//                    isTextSubscript=false
//                if(tmpFormattedString.indexOf("vertical-align:super")!=-1)
//                    isTextSuperscript=true
//                else
//                    isTextSuperscript=false
//            }

            wrapMode: TextEdit.Wrap

            textFormat: TextEdit.RichText

         }

    }  //end of columnlayout

    Dialog {
            id: colorPicker
            hasActions: true
            title: "Pick font color"
            positiveButtonText: "Done"

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
                        border.width: modelData === "white" ? Units.dp(2) : 0
                        border.color: Theme.alpha("#000", 0.26)

                        Ink {
                            anchors.fill: parent

                            onPressed: {
                                newMailTextEdit.color = parent.color
                            }
                        }
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

    FileDialog{

        id:attaPickerDialog

        title: "Please choose attachments"

        folder: shortcuts.desktop

        selectMultiple: true

        onAccepted:{
            attachmentList=String(attaPickerDialog.fileUrls).split(',')
            attachmentRowLayout.visible=true
        }
        onRejected:{
            attachmentList=[]
        }
    }
}
