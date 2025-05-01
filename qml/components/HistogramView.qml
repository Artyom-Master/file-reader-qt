import QtQuick
import QtQuick.Layouts

// Histogram Area
Rectangle {
    id: histogramArea

    property int barMinimumWidth: 30

    color: "#f0f0f0"
    border.color: "#cccccc"
    border.width: 1

    RowLayout {
        id: histogramRow
        anchors.fill: parent
        anchors.margins: variableMarginSize * 2
        spacing: variableMarginSize

        Repeater {
            id: histogramRepeater

            model: wordsCounterModel

            HistogramBar {
                barWord: word
                barCount: count
                barMaxCount: maxWordCount

                Layout.fillWidth: true
                Layout.minimumWidth: histogramArea.barMinimumWidth
                Layout.preferredWidth: 0

                //Layout.fillHeight: true

                //Layout.preferredWidth: histogramArea.width / wordsCounterModel.count
                Layout.preferredHeight: histogramArea.height
            }
        }
    }
}
