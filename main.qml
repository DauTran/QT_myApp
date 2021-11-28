import QtQuick 2.5
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.2

import org.mymodel 1.0

Window {
    id: window
    visible: true
    width: 550
    height: 600
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

        property bool status: false // to visible or invisibl the check box of each element
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
                footer.visible = visibleButton;
                rootPage.status = false;
                headerMenuBtn.enabled = visibleButton;
            }
        }
        Connections
        {
            target: subWindowInsert
            onAccepted:
            {
                if(!myModel.addFolder(subWindowInsert.check))
                {
                    subWindowFailAddFolder.open();
                }
            }
        }

        Menu {
            id: optionMenu
            bottomMargin: 50
            MenuItem {
                text: qsTr("Select All")
                onTriggered: myModel.selecAllItems();
            }
            MenuItem {
                text: qsTr("Deselect All")
                onTriggered: myModel.deselecAllItems();
            }
        }


        footer:RowLayout
            {
                id: footer
                spacing: 0
                height: 30
                visible: false
                Button
                {
                    id: footerOption
                    property bool status: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    display: AbstractButton.TextUnderIcon
                    text: "..."
                    font.pixelSize: 20
                    highlighted: false
                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                            if(footer.height == 50)
                            {
                                footer.height = 30;
                                if(optionMenu.visible)
                                {
                                    optionMenu.close();
                                }
                            }
                            else
                            {
                                footer.height = 50;
                            }

                            if(rootPage.status)
                            {

                                optionMenu.popup(Qt.point(0, footer.y));
                            }
                        }

                        hoverEnabled: true

                        onEntered: {
                            footerOption.highlighted = true;
                        }

                        onExited: {
                            footerOption.highlighted = false;
                        }
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
                    enabled: true
                    highlighted: false
                    MouseArea{
                        anchors.fill: parent
                        onClicked:
                        {
                            searchBar.state = "NORMAL";
                            rootPage.status = !rootPage.status;
                            if(rootPage.status)
                            {
                                myModel.stopTimer();
                            }
                            else
                            {
                                myModel.startTimer();
                            }
                        }
                        hoverEnabled: true

                        onEntered: {
                            footerChoice.highlighted = true;
                        }

                        onExited: {
                            footerChoice.highlighted = false;
                        }
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
                    highlighted: false
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            console.log("search");
                            if(searchBar.state == "NORMAL")
                            {
                                searchBar.state = "SEARCH";
                                myModel.stopTimer();
                            }
                            else
                            {
                                searchBar.state = "NORMAL"
                                myModel.startTimer();
                            }
                        }
                        hoverEnabled: true
                        onEntered: {
                            footerSearch.highlighted = true;
                        }

                        onExited: {
                            footerSearch.highlighted = false;
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
                    highlighted: false
                    MouseArea
                    {
                        anchors.fill: parent
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
                        hoverEnabled: true

                        onEntered: {
                            footerDelete.highlighted = true;
                        }

                        onExited: {
                            footerDelete.highlighted = false;
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
                    enabled: true
                    highlighted: false

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            subWindowInsert.open();
                        }
                        hoverEnabled: true

                        onEntered: {
                            footerNewFolder.highlighted = true;
                        }

                        onExited: {
                            footerNewFolder.highlighted = false;
                        }
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
//                        if(text == "All")
//                        {
////                            console.log("all ");
//                            myModel.setPassAll();
//                        }
//                        else if(text == "File")
//                        {
////                            console.log("file ");
//                            myModel.setPassFile();
//                        }
//                        else if(text == "Folder")
//                        {
////                            console.log("folder")
//                            myModel.setPassFolder();
//                        }
                        switch(model.index)
                        {
                        case 0:

                            console.log("file ");
                            myModel.setPassFile();
                            break;

                        case 1:
                            console.log("folder");
                            myModel.setPassFolder();
                            break;

                        default:
                            console.log("all ");
                            myModel.setPassAll();
                        }
                    }
                }
            }

            model: ListModel
            {
                ListElement
                {
                    text: qsTr("File")
                }
                ListElement
                {
                    text: qsTr("Folder")
                }
                ListElement
                {
                    text: qsTr("All")
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

    Dialog
    {
        id: subWindowFailAddFolder
        title: "Add the folders"
        Text
        {
            text: qsTr("You can not create folder " + txtInput.text);
        }
        standardButtons: StandardButton.Ok
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






















