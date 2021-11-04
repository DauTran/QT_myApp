import QtQuick 2.0
import org.mymodel 1.0

Item{
    id: structFile
    height: 30
    width: view.width
    //                border.color: Qt.darker(color)
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
        anchors { left:image.right; verticalCenter: image.verticalCenter; leftMargin: 5; rightMargin: 5}
        width: parent.width - 100

        elide: Text.ElideRight
        font.pixelSize: 24
        MouseArea {
            anchors.fill: parent
            onClicked:
            {
                myModel.changeData(model.index);
                _view.currentIndex = index;
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
        anchors { right: parent.right; rightMargin: 5 }
        MouseArea {
            anchors.fill: parent
            onClicked:
            {
                myModel.removeData(model.index);
            }
        }
    }
}
