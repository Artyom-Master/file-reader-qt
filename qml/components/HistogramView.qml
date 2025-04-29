import QtQuick
import QtQuick.Layouts

// Histogram Area
Rectangle {
    id: histogramArea
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#f0f0f0"
    border.color: "#cccccc"
    border.width: 1

    Row {
        id: histogramRow
        anchors.fill: parent
        anchors.margins: variableMarginSize * 2
        spacing: variableMarginSize

        Repeater {
            model: wordsCounterModel  // Assumes wordModel is a ListModel with 'word' and 'count' roles

            delegate: HistogramBar {
                count: wordCount
                maxCount: maxWordCount
                word: wordValue
            }
        }
    }
}
