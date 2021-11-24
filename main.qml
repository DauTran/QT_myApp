import QtQuick 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.2

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
            id: toolBar
            width: parent.width
            height: 36
            ToolButton
            {
                id: headerMenuBtn
                height: parent.height
                width: this.height
                indicator: Image{
                    width: parent.width*0.6
                    height: parent.height*0.6
                    anchors.centerIn: parent
                    source: "images/menu.png"
                }
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 0
                }
                onClicked: drawer.open()
            }

            Label{
                anchors.centerIn: parent
                text: myModel.path
                font.pixelSize: 20
                width: parent.width - headerMenuBtn.width - btnBack.width
                elide: Label.ElideLeft
//                onActiveFocusChanged:
//                {
//                    console.log("changed width");
//                }
            }

            Button {
                id: btnBack
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                text: "Back"
                visible: false
                background: Rectangle {
                    implicitWidth: 30
                    implicitHeight: 30
                    color: btnBack.down ? "#d6d6d6" : "#f6f6f6"
                    border.color: "#26282a"
                    border.width: 1
                    radius: 4
                }
                onClicked:
                {
                    myModel.changeDirUp();
                }
            }
        }
       Rectangle
        {
            id: searchBar
            width: parent.width
            border.color: "blue"
            color: "white"
            smooth: true
            clip: true
            state: "NORMAL"
            states: [
                State { name: "NORMAL"; PropertyChanges{ target: searchBar; height: 10; opacity: 0 } },
                State { name: "SEARCH" ;PropertyChanges{ target: searchBar; height: 35; opacity: 100 } }
            ]
//            RowLayout
//            {
//                width: parent.width
//                height: parent.height
                TextField {
                    id: txtInput;
                    placeholderText: qsTr("Type here...")
                    width: parent.width - btnFind.width
                    anchors.left: parent.left
                }
                Button
                {
                    id: btnFind
                    text: "Find"
                    anchors.right: parent.right
                    Layout.fillWidth: true
                    background: Rectangle
                    {
                        implicitWidth: 35
                        implicitHeight: 35
                        color: btnFind.down ? "#d6d6d6" : "#F1C40F"
                    }

                    onClicked: { myModel.search(txtInput.text);}
                }
        }

        property bool status: false
        ListView {
            id: _viewCenter
            anchors.top: searchBar.bottom
            height: parent.height - toolBar.height - searchBar.height - footer.height
            width: parent.width
            model: MyModel { id: myModel}
            focus:true // enable using keyboard
            clip: true // enable cut element in the end of row
            currentIndex: -1
            highlight: Rectangle {color: "lightsteelblue" }

            delegate: Item
            {
                id: myDelegate
                height: 30
                width: parent.width

                Rectangle{
                    id: checkListElement
                    visible: rootPage.status
                    height: parent.height
                    z : 1
                    width: 30
                    anchors { topMargin: 0; leftMargin: 5; rightMargin: 5}

                    CheckBox
                    {
                        id: boxItem
                        anchors.centerIn: parent
                        indicator.width: 25
                        indicator.height: 25

                        checked: model.check
                        onClicked: { myModel.toggelStatus(model.index);}
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
                    anchors { right: parent.right; verticalCenter: image.verticalCenter; leftMargin: 5; rightMargin: 5}
                    width: myDelegate.width - 100

                    elide: Text.ElideRight
                    font.pixelSize: 24
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {
                        _viewCenter.currentIndex = index;
                    }

                    onDoubleClicked:
                    {
                        rootPage.status = false
                        myModel.changeData(model.index);
                    }
                }
            }
        }
        Text {
            id: txtInfo
            anchors.top: searchBar.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("No data to display!")
            visible: false
            font.pixelSize: 25
            font.bold: true
        }
        Connections
        {
            target: myModel
            onCountChanged:
            {
                txtInfo.visible =  visibleData;
            }
        }

        Connections
        {
            target: myModel
            onInRoot:
            {
                btnBack.visible =  visibleButton;
                footerChoice.enabled = visibleButton;
                footerNewFolder.enabled = visibleButton;
            }
        }
        Connections
        {
            target: subWindowInsert
            onAccepted:
            {
                console.log("I got " + subWindowInsert.check);
                myModel.addFolder(subWindowInsert.check);
            }
        }

        Menu {
            id: optionMenu
            MenuItem
            {
                text: "Select All"
            }
        }

        footer:RowLayout
            {
                id: footer
                spacing: 0
                height: 30
                Button
                {
                    id: footerOption
                    property bool status: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    display: AbstractButton.TextUnderIcon
//                    highlighted: true
//                    flat: true
                    text: "..."
                    font.pixelSize: 20
                    onClicked:
                    {
                        optionMenu.popup(Qt.point(0, footer.y - 80 + subWindowConfirmDelete.height));
                    }
                }
                Button
                {
                    id: footerChoice
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    icon.source: "images/singleChoice.png"
                    display: AbstractButton.TextUnderIcon
                    text: "select"
                    enabled: false
                    onClicked:
                    {
                        rootPage.status = !rootPage.status;
                    }
                }
                Button
                {
                    id: footerSearch
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    icon.source: "images/search.png"
                    display: AbstractButton.TextUnderIcon
                    text: "search"
                    enabled: !rootPage.status
                    onClicked: {
                        console.log("search");
                        if(searchBar.state == "NORMAL")
                        {
                            searchBar.state = "SEARCH"
                        }
                        else
                        {
                            searchBar.state = "NORMAL"
                        }
                    }
                }
                Button
                {
                    id: footerDelete
                    Layout.fillWidth: true
                    icon.source: "images/remove.png"
                    Layout.fillHeight: true
                    display: AbstractButton.TextUnderIcon
                    text: "delete"
                    enabled: rootPage.status
                    onClicked:
                    {
                        subWindowConfirmDelete.open();

                    }
                    Connections
                    {
                        target: subWindowConfirmDelete
                        onAccepted:
                        {
                           myModel.deleteSelection();
                        }
                    }
                }

                Button
                {
                    id: footerNewFolder
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    icon.source: "images/addFolder.png"
                    display: AbstractButton.TextUnderIcon
                    text: "add"
                    enabled: false
                    onClicked:
                    {
                        console.log("add folder Pressed");
                        subWindowInsert.open();
                    }
                }
            }
//        }
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

    Dialog
    {
        id: subWindowInsert
        property string check : inputText.text
        title: "Add new folder"
        TextField
        {
            id: inputText
            placeholderText: qsTr("Enter the name of folder")
        }
        standardButtons: StandardButton.Save | StandardButton.Cancel
    }

    Dialog
    {
        id: subWindowConfirmDelete
        title: "Delete the folders"
        Text
        {
            text: qsTr("Are you sure?")
        }
        standardButtons: StandardButton.Ok | StandardButton.Cancel
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






















