import QtQuick 2.0

Item {
    width: 1600
    height: 700

    ListView {
        id: listView
        x: 0
        y: 10
        width: 1580
        height: 700
        delegate: Item {
            x: 5
            width: 80
            height: 200
            Row {
                id: row1
                spacing: 10
                Image {
                    width: 120
                    height: 160
                    source: imagesource
                    fillMode: Image.PreserveAspectFit
                }

                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                }

                Text {
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12
                    font.bold: true
                }
            }
        }
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
                imagesource: "5bf51701N280ba849.jpg"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
    }
    function addOne() {
                model.append(
                            {
                                "name": "MX3",
                                "colorCode": "blue"
                            }
                )
            }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.5}
}
##^##*/
