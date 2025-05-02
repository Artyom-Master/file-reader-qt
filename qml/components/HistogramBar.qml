import QtQuick
import QtQuick.Layouts

ColumnLayout {
    id: histogramBar

    property int barCount
    property int barMaxCount
    property string barWord

    Item {
        Layout.fillHeight: true
    }

    //Word occurrencies in file
    Text {
        id: barCountLabel

        text: barCount
        font.pixelSize: mainView.appFontSize
        elide: Text.ElideRight

        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
    }

    TextMetrics {
        id: barCountLabelMetrics

        font: barCountLabel.font
        text: barCountLabel.text
    }

    //Word bar in histogram
    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: barMaxCount > 0
            ? (barCount / barMaxCount) * (histogramBar.Layout.preferredHeight - barCountLabelMetrics.boundingRect.height - barWordLabelMetrics.boundingRect.height)
            : 0

        color: "royalblue"
        border.color: "black"
        border.width: 1
    }

    //Word from file
    Text {
        id: barWordLabel

        text: barWord
        font.pixelSize: mainView.appFontSize
        elide: Text.ElideRight

        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
    }

    TextMetrics {
        id: barWordLabelMetrics

        font: barWordLabel.font
        text: barWordLabel.text
    }
}
