import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Material 0.2
import Material.Extras 0.1
import Material.ListItems 0.1 as ListItem
import QtWebEngine 1.2

ApplicationWindow {
    id : mainWindow

    title: "MicroMailClient"

    visible: true

    property bool isLargeWindow: width > Screen.desktopAvailableWidth/2

    property int selectedMailIndex: -1

    property int selectedFolder: 0

    property string globalBackgroundColor: "whitesmoke"

    property string sidebarColor: Palette.colors["blue"]["200"]

    property double iconScale: 0.8

    property string contentFont: "微软雅黑"

    property string host : "pop.qq.com";

    property string user : "375670450@qq.com";

    property string passwd : "sftkpahwbroabhjg";

    property string port: "110";

    theme {
        primaryColor: "blue"
        accentColor: "red"
        primaryDarkColor: "gray"
        tabHighlightColor: "Zwhite"
    }

    initialPage: Tab{
        sourceComponent: mainWindowComponent

    }

    Component{
        id: mainWindowComponent

        Rectangle{

            id: backgroundRectangle

            color: Theme.backgroundColor

            anchors.fill: parent

            MouseArea{

                anchors.fill: parent
                property variant previousPosition
                onPressed: {
                    previousPosition = Qt.point(mouseX, mouseY)
                }
                onPositionChanged: {
                    if( pressedButtons == Qt.LeftButton ){
                        mainWindow.x += mouseX - previousPosition.x
                        mainWindow.y += mouseY - previousPosition.y
                    }
                }
               onClicked: {
                   menuSidebar.expanded = false

               }
            }

            Sidebar{
                id: menuSidebar  // 完整的侧边栏
                expanded: true
                width: Units.dp(200)

                backgroundColor: sidebarColor

                ColumnLayout{
                    anchors.fill: parent
                    spacing: Units.dp(2)

                    width: parent.width

                    Button{
                        width: height * 1.2
                        Image {
                            source: "/icons/menu"
                            scale: iconScale
                        }
                        onClicked:  menuSidebar.expanded = !menuSidebar.expanded
                    }

                    MyButton{
                        id: newMailButton
                        Layout.fillWidth: true
                        source: "/icons/add"
                        label:  "New Mail"

                        onClicked: {
                            pageLoader.newMail()
                        }

                    }

                    MyButton{
                        Layout.fillWidth: true
                        source: "/icons/person"
                        label: "Accounts"

                    }

                    MyButton{
                        Layout.fillWidth: true
                        source: "/icons/person_add"
                        label: "Add Account"

                        onClicked: {
                            loginDialog.visible = true
                            loginDialog.show()
                        }

                    }

                    ListItem.SectionHeader{
                        id: foldersSectionHeader

                        Layout.fillWidth: true

                        RowLayout{

                            Image {
                                id: foldersImage
                                scale: iconScale
                            }

                            Label{
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Folders"
                            }

                        }

                        state: "open"
                        expanded: true

                        states:[
                            State{
                                name: "open"
                                PropertyChanges {
                                    target: foldersImage
                                    source: "/icons/folder_open"
                                }
                            },

                            State{
                                name: "close"
                                PropertyChanges {
                                    target: foldersImage
                                    source: "/icons/folder"
                                }
                            }

                        ]

                        onClicked: {
                            state == "close" ? state = "open" : state = "close";

                        }

                    }

                    ColumnLayout {
                        anchors.top: foldersSectionHeader.bottom
                        width:  foldersSectionHeader.width

                        Repeater{
                            id: foldersList

                            model: mailListModel.folders
        //                    model: temp
                            Layout.fillWidth: true
                            delegate: foldersListDelegate
                            onModelChanged: console.log("model = " + model)
                        }

                    }

                    Component{
                        id: foldersListDelegate
                        ListItem.Standard{
                            width: foldersSectionHeader.width
                            visible: { foldersSectionHeader.expanded }

        //                    backgroundColor: mailListModel.folderIndex == index ?


                            Label{
                                text: model.modelData
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: Units.dp(10)
                                font.pixelSize: Units.dp(13)
                            }
                            onVisibleChanged: console.log("listitem visible changed: " + visible)
                            onClicked: mailListModel.buildMailList(index)
                        }
                    }

                }

            }

            Rectangle{

                id: thinMenuSidebar             // 小的侧边栏

                width: menuSidebar.expanded ? 0 : Units.dp(35)

                height: parent.height

                color: sidebarColor

                anchors.left: menuSidebar.right

                Column {
                    spacing: Units.dp(10)

                    anchors.fill : parent

                    Button{

                        width: parent.width
                        height: width
                        Image{
        //                    scale: iconScale
                            anchors.fill: parent
                            source: "/icons/menu"
                        }

                        onClicked: {
                            menuSidebar.expanded = !menuSidebar.expanded
                        }
                    }

                }

            }



            Rectangle{

                id: mailListColumn

                anchors.left: thinMenuSidebar.right

                anchors.leftMargin: Units.dp(4)

                width: isLargeWindow ? Units.dp(400) : parent.width - thinMenuSidebar.width

                height: parent.height

                color: Theme.backgroundColor

                ListView{

                    id:mailListView

                    anchors.fill: parent

                    width: parent.width

                    delegate:mailListDelegate

                    model: mailListModel

                    header : Rectangle{
                        id: mailListHeader
                        height: Units.dp(35)
                        width: mailListView.width

                        Rectangle{
                            id : searchTextRectangle
                            height: mailListHeader.height
                            width: parent.width - mailListHeaderButtonRectangle.width
                            color: globalBackgroundColor
                            border.color: searchTextField.activeFocus ? "lightgray" : color

                            TextField{

                                id: searchTextField
                                text: "search"
                                height: mailListHeader.height
                                anchors.right: searchButton.left
                                width: parent.width - searchButton.width
                                font.pointSize: Units.dp(14)
                                font.bold: false
                                font.family: contentFont

                            }

                            Button{
                                id : searchButton
                                anchors.right: parent.right
                                width: Units.dp(40)

                                Image{
                                    scale: iconScale
                                    id : searchButtonImage
                                    anchors.fill: parent
                                    anchors.margins: Units.dp(4)
                                    source: "/icons/search"

                                }

                                onClicked: {
                                    console.log("Search Button Clicked")
                                }

                            }

                        }

                        Rectangle{
                            id: mailListHeaderButtonRectangle
                            anchors.left: searchTextRectangle.right
                            height: mailListHeader.height
                            width: Units.dp(80)

                            color: globalBackgroundColor

                            Button{
                                id: refreshButton
                                width: parent.width/2
                                Image{
                                    scale: iconScale
                                    anchors.fill: parent
                                    anchors.margins: Units.dp(4)
                                    source: "/icons/refresh"
                                }
                                onClicked: {
                                    console.log("refresh button onclick")
                                    mailListModel.login(user, passwd, host, port)
                                }

                            }
                            Button{
                                anchors.left: refreshButton.right
                                id: selectButton

                                width: parent.width/2
                                Image{
                                    scale: iconScale
                                    anchors.fill: parent
                                    anchors.margins: Units.dp(4)
                                    source: "/icons/select"
                                }
                                onClicked: {
                                        console.log("select button onclick")
//                                    alertNoTitleBar.show()
                                }
                            }
                        }

                        ProgressBar{

                            id: mailListProgressBar
                            width: parent.width
                            anchors.bottom : parent.bottom

                            value: mailListModel.progress

        //                    onValueChanged: console.log("valueChanged:"+value)

                        }

                    }

                    section.property: "mail_datetime"        // should be datetime
                    section.criteria: ViewSection.FullString
                    section.delegate: mailListSectionDelegate
                }

        /*
            Invisible Components of mailListColumn
        */
                Component{
                    id: mailListSectionDelegate
                    ListItem.Subheader{
                        backgroundColor: Theme.backgroundColor
                        height: Units.dp(40)
                        Label{
                            anchors.verticalCenter: parent.verticalCenter
                            text: section
                            font.pixelSize: Units.dp(14)
                        }
                        ThinDivider{
                            anchors {
                                left: parent.left
                                right: parent.right
                                bottom: parent.bottom
                            }
                            visible: true
                        }
                    }
                }

                Component{
                    id: mailListDelegate

                    ListItem.Standard{
                        id: mailListItem

                        width: mailListColumn.width
                        height: Units.dp(100)

                        backgroundColor: selectedMailIndex == index ? "silver" : globalBackgroundColor

                        onClicked: {
                            model.mail_isread = true;           // update isread
                            selectedMailIndex = index;

                            pageLoader.changeMail(index)

//                            pageLoader.item.mailWebView.loadHtml(mailListModel.getHTMLContent(index));
                        }

                       Rectangle{
                           id: isreadRectangle
                           height: parent.height

                           width: Units.dp(5)
                           color: model.mail_isread ? mailListItem.backgroundColor : "green"
                       }

                       ColumnLayout{
                            anchors.left: isreadRectangle.right
                            anchors.margins: Units.dp(5)
                            spacing: Units.dp(5)

                            Label{
                                id: mailSender
                                text: model.mail_sender.split('@')[0]
                                font.bold: true
                                Layout.fillWidth: true
                                font.pixelSize: Units.dp(20)
                            }

                            Label{
                                id: mailTitle
                                anchors.leftMargin: Units.dp(5)
                                Layout.fillWidth: true
                                text: model.mail_subject

                                font.family: contentFont

                                wrapMode:  Text.WordWrap
                            }

                            Label{
                                id: mailContent
                                anchors.leftMargin: Units.dp(5)
                                Layout.fillWidth: true

                                text: model.mail_content.split('\n')[0]
                                font.family: contentFont
                                wrapMode: Text.WordWrap

                            }

                       }


                        ThinDivider{
                            anchors {
                                left: parent.left
                                right: parent.right
                                bottom: parent.bottom
                            }
                            visible: true
                        }

                    }
                }
            }


            Loader{

                id: pageLoader

                anchors.left: mailListColumn.right

                height: parent.height

                width: parent.width - mailListColumn.x - mailListColumn.width       // How to simplify?

                //                visible: selectedMailIndex >= 0

                source: Qt.resolvedUrl("NewMailEdit.qml")
//                source: Qt.resolvedUrl("MailView.qml")

                function test() {
                    console.log("parent.width = " + parent.width)
                    console.log("mailListColumn.x = " + mailListColumn.x)
                    console.log("mailListColumn.width = " + mailListColumn.width)
                }
                function changeMail(index) {
                    item.loadHtml(mailListModel.getHTMLContent(index))
                }

                function newMail(){
                    console.log("in Function newMail")
                    setSource(Qt.resolvedUrl("NewMailEdit.qml"))
                }

            }

            AccountLogin{
                id: loginDialog

                visible: false

                signInButton.onClicked: mailListModel.login(user, passwd, host, port)

                onClosed: visible = false
            }

        }   // BackgroundRectangle
    }

}

