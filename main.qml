import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem


ApplicationWindow {



    id : mainWindow

    title: "MicroMailClient"

    visible: true

    property bool isLargeWindow: width > Screen.desktopAvailableWidth/2

    property int selectedIndex: 0

    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "Zwhite"
    }

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
        onClicked: menuSideBar.expanded = false;
    }

    Rectangle{

        id: _menuSideBar

        color : menuSideBar.backgroundColor

        width: 30
        height: parent.height

        anchors.left: parent.left

        Column {
            spacing: 10

            anchors.fill : parent

            Button{

                width: parent.width
                Image{
                    anchors.fill: parent
                    fillMode: Image.Pad
                    source: "/icons/menu"

                }
                onClicked: menuSideBar.expanded = true
            }

        }
    }


    Sidebar{
        id : menuSideBar

        backgroundColor: "lightgray"

        expanded: false

        contents: [
            Rectangle{
                Button{ text:"asd" }
            },
            Action{
                iconName: "icons/menu"
                name: "expandSidebar"
            }

        ]
    }



    Rectangle{

        id: mailListColumn

        anchors.left: _menuSideBar.right

        width: isLargeWindow ? 400 : parent.width - _menuSideBar.width

        height: parent.height

        ListView{

            id:mailListView

            anchors.fill: parent

            width: parent.width

            delegate:mailListDelegate

            model: mailListModel

            header : Rectangle{
                    id: mailListHeader
                    height: 35
                    width: mailListView.width

                    Rectangle{
                        id : searchTextRectangle
                        height: mailListHeader.height
                        width: parent.width - mailListHeaderButtonRectangle.width

                        TextField{
                            id: searchTextField
                            text: "search"
                            height: mailListHeader.height
                            anchors.right: searchButton.left
                            width: parent.width - searchButton.width
                        }

                        Button{
                            id : searchButton
                            anchors.right: parent.right
                            width: 40
                            Image{
                                id : searchButtonImage
                                anchors.fill: parent
                                anchors.margins: 4
                                source: "/icons/search"
                            }
                        }

                    }


                    Rectangle{
                        id: mailListHeaderButtonRectangle
                        anchors.left: searchTextRectangle.right
                        height: mailListHeader.height
                        width: 80

                        color: "lavender"

                        Button{
                            id: refreshButton
                            width: parent.width/2
                            Image{
                                anchors.fill: parent
                                anchors.margins: 4
                                source: "/icons/refresh"
                            }
                        }
                        Button{
                            anchors.left: refreshButton.right
                            id: selectButton
                            width: parent.width/2
                            Image{
                                anchors.fill: parent
                                anchors.margins: 4
                                source: "/icons/select"
                            }
                        }
                    }


                }

            section.property: "mail_subject"        // should be datetime
            section.criteria: ViewSection.FullString
            section.delegate: mailListSectionDelegate
        }

        ListView{
            width: 100
            height: 100
            anchors.right: parent.right
            anchors.top: parent.top
            model: mailListModel.Get(0)

            delegate :Component{
                ListItem.Standard{
                    width: 100
                    height: 100

                    backgroundColor: "red"
                    Label{
                        text: "subject" + model.subject
                    }
                }

            }
        }



/*
    Invisible Components
*/
        Component{
            id: mailListSectionDelegate
            ListItem.Subheader{
                backgroundColor: "ghostwhite"

                height: 40
                Label{
                    anchors.verticalCenter: parent.verticalCenter
                    text: section
                    font.pixelSize: 14
                }
            }
        }

        Component{
            id: mailListDelegate

            ListItem.Standard{

                width: mailListColumn.width
                height: 100

                backgroundColor: selectedIndex == index ? "silver" : "white"

                onClicked: {
                    selectedIndex = index;
                }

               Rectangle{
                   id: isreadRectangle
                   height: parent.height
                   width: 5
                   color: model.mail_isread ? parent.backgroundColor : "green"
               }

                Label{
                    id: mailTitle
                    anchors.left: isreadRectangle.right
                    anchors.margins: 5
                    text: model.mail_subject
                    font.bold: true
                    font.pixelSize: 20
                }

                Label{
                    anchors.top: mailTitle.bottom
                    anchors.left: mailTitle.left
                    anchors.margins: 5

                    id: mailContent
                    text:model.mail_content
                }

                border.color: "ghostwhite"

            }

        }

    }

}
