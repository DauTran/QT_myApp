import QtQuick 2.5
import QtQuick.Layouts 1.2

Item {
    id: root
    width: ListView.view.width
    height: 48
    property alias text: label.text
    property alias color: label.color

    signal clicked()
    signal remove()

    //Border for files/folders
    Rectangle {
        anchors.fill: parent
        color: '#ffffff'
        opacity: 0.2
        border.color: Qt.darker(color)
    }

    RowLayout
    {
        anchors.fill: parent
        anchors.leftMargin: 30
        anchors.rightMargin: 8
        spacing: 20

        // icon file/folder
        Item {
            id: iconItem
            Layout.fillHeight: true
            Layout.preferredWidth: iconItem.width
            Image
            {
                id:iconFile
                anchors.centerIn: parent
//                source: '../images/folder.png'
//                source:
            }

        }

        // file/folder name
        Item
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: label
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 8
                font.pixelSize: 24
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    root.clicked()
                }
            }
        }

        // icon remove
        Item
        {
            id: removeItem
            Layout.fillHeight: true
            Layout.preferredWidth: iconRemove.width
            Image {
                id: iconRemove
                anchors.centerIn: parent
                source: "../images/remove.png"
            }
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    root.remove()
                    console.log("delete push %d", index)
                    console.warn("txt: " + index.text)
                    console.log("width = %d", iconFile.width)
                }
            }
        }

    }
}
