import QtQuick
import QtQuick.Layouts

// Histogram Area
Rectangle {
    id: histogramView

    property int barMinimumWidth: 30
    property int barsSpacing: 3
    property double numbersOfOccurenciesLabelWidth: numbersOfOccurenciesLabelMetrics.tightBoundingRect.height

    color: "whitesmoke"
    border.color: "lightgrey"
    border.width: 1

    RowLayout {
        anchors.fill: parent
        anchors.margins: mainView.appMargin
        spacing: mainView.appMargin

        //Numbers of words occurrencies in file label
        Text {
            id: numbersOfOccurenciesLabel

            text: "Numbers of occurencies"
            font.pixelSize: mainView.appFontSize

            Layout.maximumWidth: histogramView.numbersOfOccurenciesLabelWidth
            horizontalAlignment: Text.AlignHCenter
            rotation: 270
        }

        TextMetrics {
            id: numbersOfOccurenciesLabelMetrics

            font: numbersOfOccurenciesLabel.font
            text: numbersOfOccurenciesLabel.text
        }

        ColumnLayout {
            RowLayout {
                spacing: histogramView.barsSpacing

                //Histogram bars
                Repeater {
                    model: wordsCounterModel

                    HistogramBar {
                        barWord: word
                        barCount: count
                        barMaxCount: maxWordCount

                        Layout.fillWidth: true
                        Layout.minimumWidth: histogramView.barMinimumWidth
                        Layout.preferredHeight: histogramView.height - wordsLabelMetrics.boundingRect.height - appMargin * 4
                    }
                }
            }

            //Words from file label
            Text {
                id: wordsLabel

                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter

                text: "Words"
                font.pixelSize: mainView.appFontSize
            }

            TextMetrics {
                id: wordsLabelMetrics

                font: wordsLabel.font
                text: wordsLabel.text
            }
        }
    }
}
