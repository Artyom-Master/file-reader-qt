import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "../components" as Components

ColumnLayout {
    id: wordStatsHistogramm

    readonly property int relativeCoefficient: (parent.width + parent.height) * 0.006
    readonly property int variableTextSize: (relativeCoefficient > 14 ? relativeCoefficient : 14)
    readonly property int variableMarginSize: (relativeCoefficient > 5 ? relativeCoefficient : 5)
    readonly property int variableItemSize: (relativeCoefficient * 3 > 30 ? relativeCoefficient * 3 : 30)

    spacing: variableMarginSize
    anchors.fill: parent
    anchors.margins: variableMarginSize

    //Histogram View
    Components.HistogramView {
        id: histogramView

        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumWidth: wordsCounterModel.length > 0
                             ? (histogramView.barMinimumWidth * wordsCounterModel.length
                                + (wordsCounterModel.length - 1) * variableMarginSize
                                + variableMarginSize * 4)
                             : 200
        Layout.minimumHeight: 200
    }

    Text {
        text: qsTr(wordsCounterModel.statusInfoText)
        font.pixelSize: variableTextSize
    }

    // Progress Bar
    ProgressBar {
        id: progressBar
        Layout.fillWidth: true
        implicitHeight: variableItemSize
        from: 0
        to: 100
        value: wordsCounterModel.countProgress  // Bind this to a property that updates with processing progress
    }

    // Control Buttons
    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: variableMarginSize

        Button {
            text: qsTr("Open")
            font.pixelSize: variableTextSize
            onClicked: fileDialog.open()
        }

        Button {
            text: qsTr("Start")
            font.pixelSize: variableTextSize
            onClicked: controller.startProcessing()  // Assumes 'controller' is a context property with this method
        }

        Button {
            text: qsTr("Pause")
            font.pixelSize: variableTextSize
            onClicked: controller.pauseProcessing()
            enabled: controller.canPause  // Assumes 'canPause' is a property indicating pause capability
        }

        Button {
            text: qsTr("Cancel")
            font.pixelSize: variableTextSize
            onClicked: controller.cancelProcessing()
            enabled: controller.canCancel  // Assumes 'canCancel' is a property indicating cancel capability
        }
    }

    // File Dialog
    FileDialog {
        id: fileDialog
        title: qsTr("Select a Text File")
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted:
        {
            console.log("File url you chose: " + selectedFile)
            controller.openFile(selectedFile)  // Assumes 'openFile' method in controller
        }
    }
}
