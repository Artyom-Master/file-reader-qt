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
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Text {
        text: barWord
        font.pixelSize: variableTextSize
        horizontalAlignment: Text.AlignHCenter
        Layout.alignment: Qt.AlignHCenter
        elide: Text.ElideRight
        Layout.fillWidth: true
    }
}
