import QtQuick 2.6
import QtQuick.Window 2.2
import Material 0.2
import Material.ListItems 0.1 as ListItem


ApplicationWindow {

    id : mainWindow

    title: "MicroMailClient"

    visible: true

//    flags:Qt.FramelessWindowHint

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
        color : menuSideBar.backgroundColor

        width: 30
        height: parent.height

        anchors.left: parent.left

        Column {

            anchors.fill : parent

            Button{

                width: parent.width
                text:"asd"
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
                iconName: "awesome/bar"
                name: "expandSidebar"
            }

        ]
    }

}
