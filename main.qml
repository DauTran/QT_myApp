import QtQuick 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.1

import org.mymodel 1.0
import "common"

Window {
    id: window
    visible: true
    width: 550
    height: 480
    title: qsTr("Hello World")
    minimumHeight: 500
    minimumWidth: 300

    Page
    {
        id: rootPage
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

        property bool status: false
        ListView {
            id: _viewCenter
            anchors.fill: parent
            model: MyModel { id: myModel}
            focus:true // enable using keyboard
            clip: true // enable cut element in the end of row
            currentIndex: -1
            highlight: Rectangle { color: "lightsteelblue" }
            layoutDirection: Qt.RightToLeft

            delegate: Item
            {
                id: myDelegate
                height: 30
                width: parent.width
//                property bool checked: model.check
                Item{
                    id: checkListElement
                    visible: rootPage.status
                    height: childrenRect.height
                    width: childrenRect.width
                    CheckBox
                    {
                        checked: model.check
                        onClicked: {myModel.toggelStatus(model.index);
                            console.log("Hi " + checked);
//                      anchors.fill: parent
                        }
                    }

                }
                Image {
                    id: image
                    source: model.icon
                    width: 25
                    height: 25
                    fillMode: Image.PreserveAspectFit
                    anchors { right: txtRow.left; rightMargin: 5}
                }
                Text {
                    id: txtRow
                    text: model.name
                    anchors { right: iconRemove.left; verticalCenter: image.verticalCenter; leftMargin: 5; rightMargin: 5}
                    width: parent.width - 100

                    elide: Text.ElideRight
                    font.pixelSize: 24
                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                            myModel.changeData(model.index);
                            myDelegate.ListView.view.currentIndex = index;
                            console.log("he " + myDelegate.ListView.view.currentIndex + " // " + model.index)
                        }
                    }
                }

                // icon remove
                Image {
                    id: iconRemove
                    source: "images/remove.png"
                    width: 25
                    height: 25
                    fillMode: Image.PreserveAspectFit
                    anchors { topMargin: 5; right: parent.right; rightMargin: 5 }
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
        footer: Rectangle
        {
            id: footer
            width: parent.width
            height: 30
            color: "#F4F1F0"
            Button
            {
                id: footerOption
                property bool status: false
                anchors.right: parent.right
                text: "..."
                font.pixelSize: 20
                width: this.height

                onClicked:
                {
                    console.log("ok");
                }
            }
            Button
            {
                id: footerChoice
                icon.source: "images/singleChoice.png"
                onClicked:
                { rootPage.status = !rootPage.status;
//                  footerDelete.enabled = rootPage.status
                  console.log("choice pressed")
                }
            }
            Button
            {
                id: footerSearch
                icon.source: "images/search.png"
                x: footerDelete.width + 5
                enabled: !rootPage.status
                onClicked: console.log("search");
            }
            Button
            {
                id: footerDelete
                icon.source: "images/remove.png"
                x: footerSearch.x + footerSearch.width + 5
                enabled: rootPage.status
                onClicked:
                {
                    console.log("delete Pressed ")

                    myModel.deleteSelection();
                }
            }

        }
    }

    //menubar
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
            highlight: Rectangle {
                anchors{ left: parent.left; right: parent.right}
                color: "lightsteelblue"
            }

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
                }
                ListElement
                {
                    text: qsTr("File")
                }
                ListElement
                {
                    text: qsTr("Folder")
                }
            }
            ScrollIndicator.vertical: ScrollIndicator {}
        }
    }

}


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






















