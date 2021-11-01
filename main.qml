import QtQuick 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
//import QtQuick.Controls.Material 2.1
//import "./common"


//import org.backend 1.0
//import org.model.entry 1.0
//import org.mymodel 1.0


Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Page
    {
        anchors.fill: parent
        header: ToolBar
        {
            width: parent.width
            height: 39
            ToolButton
            {
                id: menuButton
                indicator: Image{
                    height: menuButton.width * 0.6
                    width: menuButton.height * 0.6
                    anchors.centerIn: menuButton
                    source: "images/menu.png"
                }
                onClicked: drawer.open()
            }

            Label{
                anchors.centerIn: parent
                text: "File explore"
                font.pixelSize: 20
                elide: Label.ElideRight
            }
        }

        ListView {
            id: view
            anchors.fill: parent
//            orientation: ListView.Vertical
            model: myModel
            delegate: Rectangle
            {
                id: row
                height: childrenRect.height
                width: parent.width
                border.color: Qt.darker(color)

                Image {
                    id: image
                    source: model.icon
                    width: 25
                    height: 25
                    fillMode: Image.PreserveAspectFit

                    anchors { left:parent.left }
                }
                Text {
                    text: model.name
                    anchors { left:image.right; verticalCenter: image.verticalCenter; leftMargin: 5}
                    font.pixelSize: 24
                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
//                            myModel.createNewData(model.index);
//                            model.path = "C:/Users/theda/.vscode/extensions/ms-vscode.cpptools-1.7.0"
                            myModel.changeData(model.index);
//                            myModel.clear();
//                            console.log("Push")
                        }
                    }
                }

                // icon remove
                Image {
                    id: iconRemove
                    source: "../images/remove.png"
                    width: 25
                    height: 25
                    fillMode: Image.PreserveAspectFit
                    Rectangle{
                        anchors.fill: parent
                        color: "white"
                        z: -1
                    }

                    anchors { right:parent.right }
                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                            myModel.removeData(model.index);
                        }
                    }
                }
            }
        }
//        highlight: Rectangle { color: "lightsteelblue"}



//        ListView
//        {
//            id: view
//            anchors.fill: parent

//            model: DataEntryModel {id: dataEntryModel; path: "A:/Fresher C++/"}

//            delegate: ListDelegate
//            {
////              use the difined model role "display"
//                text: model.display

//                onClicked: {
//                    // make this delegate the current item, highlight
//                    view.currentIndex = index
//                    view.focus = true
//                }

//            }
//            highlight: Rectangle { color: "lightsteelblue"}
////            highlight: ListHighlight { }
//        }

    }

    Drawer
    {
        id: drawer

        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height

        ListView
        {
            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: parent.width
                text: model.text
                highlighted: ListView.isCurrentItem
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        if(text == "All")
                        {
                            myModel.passAll();
                        }
                        else if(text == "File")
                        {
                            myModel.passFile();
                        }
                        else if(text == "Folder")
                        {
                            myModel.passFolder();
                        }
                    }
                }
            }

            model: ListModel
            {
                ListElement
                {
                    text: qsTr("All")
//                    MouseArea
//                    {
//                        anchors.fill: parent
//                        onClicked:
//                        {
////                            myModel.pas
//                        }
//                    }
                }
                ListElement
                {
                    text: qsTr("File")
//                    Button
//                    {
//                        MouseArea
//                        {
//                            anchors.fill: parent
//                            onClicked:
//                            {
//                                myModel.passFile()
//                                console.log("file push");
//                            }
//                        }
//                    }
                }
                ListElement
                {
                    text: qsTr("Folder")
//                    MouseArea
//                    {
//                        anchors.fill: parent
//                        onClicked:
//                        {
//                            myModel.passFolder()
//                            console.log("folder.log");
//                        }
//                    }
                }
            }
            ScrollIndicator.vertical: ScrollIndicator {}
        }
    }

}
























