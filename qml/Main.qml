import QtQuick
import QtQuick.Layouts
import "views" as Views

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Words counter")

    minimumWidth: mainView.Layout.minimumWidth + 2 * mainView.appMargin
    minimumHeight: mainView.Layout.minimumHeight + 2 * mainView.appMargin

    Views.MainView {
        id: mainView
    }
}
