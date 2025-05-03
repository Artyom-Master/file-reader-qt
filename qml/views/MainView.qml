import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import "../components" as Components

RowLayout {
    id: mainView

    readonly property int appFontSize: 20
    readonly property int appMargin: 15
    readonly property int appButtonWidth: 200

    readonly property int minimumHistogramWidth: 300
    readonly property int minimumHistogramHeight: 300

    readonly property int progressBarHeight: 40

    spacing: appMargin
    anchors.fill: parent
    anchors.margins: appMargin

    // Control Buttons
    ColumnLayout {
        Layout.fillWidth: false
        Layout.preferredWidth: mainView.appButtonWidth
        spacing: mainView.appMargin

        Button {
            text: qsTr("Open file")
            font.pixelSize: mainView.appFontSize
            onClicked: fileDialog.open()
            enabled: controller.canOpenFile
        }

        Button {
            text: qsTr("Start")
            font.pixelSize: mainView.appFontSize
            onClicked: controller.startProcessing()
            enabled: controller.canStart
        }

        Button {
            text: qsTr(controller.pauseButtonText)
            font.pixelSize: mainView.appFontSize
            onClicked: controller.pauseProcessing()
            enabled: controller.canPause
        }

        Button {
            text: qsTr("Cancel")
            font.pixelSize: mainView.appFontSize
            onClicked: controller.cancelProcessing()
            enabled: controller.canCancel
        }

        Item {
            Layout.fillHeight: true
        }
    }

    ColumnLayout {
        //Histogram View
        Components.HistogramView {
            id: histogramView

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: wordsCounterModel.wordsCount > 0
                                 ? (barMinimumWidth * wordsCounterModel.wordsCount
                                    + (wordsCounterModel.wordsCount - 1) * barsSpacing
                                    + numbersOfOccurenciesLabelWidth + mainView.appMargin * 3)
                                 : mainView.minimumHistogramWidth
            Layout.minimumHeight: mainView.minimumHistogramHeight
        }

        //Status info
        Text {
            text: qsTr(controller.statusInfoText)
            font.pixelSize: mainView.appFontSize
        }

        //Progress Bar
        ProgressBar {
            id: progressBar

            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.preferredHeight: mainView.progressBarHeight

            from: 0
            to: 100
            value: controller.countProgress

            Behavior on value {
                SmoothedAnimation {
                    velocity: 200
                }
            }
        }
    }

    // File Dialog
    FileDialog {
        id: fileDialog
        title: qsTr("Select a Text File")
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted:
        {
            controller.openFile(selectedFile)
        }
    }
}
