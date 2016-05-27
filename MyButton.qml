import QtQuick 2.6
import QtQuick.Layouts 1.1
import Material 0.2

Button{

    property alias source : img.source

    default property alias label : btnLabel.text

    RowLayout{

        Image{
            id : img
            source: ""
            scale: iconScale
        }
        Label{
            id : btnLabel
            anchors.verticalCenter: parent.verticalCenter
            text: text
        }

    }


}

