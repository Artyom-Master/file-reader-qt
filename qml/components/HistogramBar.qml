import QtQuick
import QtQuick.Layouts

ColumnLayout {
    id: root

    property int barCount
    property int barMaxCount
    property string barWord

    // spacer pushes the bar down
    Item { Layout.fillHeight: true }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: barMaxCount > 0
            ? (barCount / barMaxCount) * (root.Layout.preferredHeight * 0.7)
            : 0
        color: "#3498db"

        Text {
            text: barCount
            font.pixelSize: variableTextSize
            Layout.fillWidth: true
            Layout.fillHeight: true
            horizontalAlignment: Text.AlignHCenter
            //Layout.alignment: Qt.AlignCenter
        }
    }

    Text {
        text: barWord
        font.pixelSize: variableTextSize
        horizontalAlignment: Text.AlignHCenter
        Layout.alignment: Qt.AlignHCenter
        //wrapMode: Text.WordWrap
        elide: Text.ElideRight
        Layout.fillWidth: true
        //width: root.width
    }
}
