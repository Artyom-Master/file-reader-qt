import QtQuick

Column {
    width: variableItemSize
    height: parent.height

    property int count
    property int maxCount
    property string word

    Rectangle {
        width: parent.width
        height: (count / maxCount) * parent.height  // maxCount should be the highest count value
        color: "#3498db"
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        text: word
        font.pixelSize: variableTextSize
        horizontalAlignment: Text.AlignHCenter
        width: parent.width
        wrapMode: Text.Wrap
    }
}
