import QtQuick 2.0

Item {
    id: element
    width: 500
    height: 200

    Image {
        id: image
        x: 8
        y: 12
        width: 120
        height: 170
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/qtquickplugin/images/template_image.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: element1
        x: 134
        y: 25
        width: 358
        height: 30
        text: qsTr("Text")
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
    }

    Text {
        id: element2
        x: 134
        y: 61
        width: 358
        height: 36
        text: qsTr("Text")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: element3
        x: 322
        y: 103
        width: 170
        height: 36
        text: qsTr("Text")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }

    Text {
        id: element4
        x: 134
        y: 103
        width: 170
        height: 36
        text: qsTr("Text")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }

}
