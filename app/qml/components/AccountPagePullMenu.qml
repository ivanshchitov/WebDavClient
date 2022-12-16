// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0

PullDownMenu {
    id: pullDownMenu

    Component.onCompleted: opacity = 1.0

    opacity: 0.0

    Behavior on opacity  {
        NumberAnimation {
            duration: 400
        }
    }

    MenuItem {
        onClicked: {
            pullDownMenu.cancelBounceBack();
            pageStack.push(Qt.resolvedUrl("../dialogs/ServiceSelectionDialog.qml"));
        }

        text: qsTr("Add an account")
    }
}
