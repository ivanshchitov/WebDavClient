// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

EditFileOrFolderNameDialog {
    property var element
    property string newName
    property string newPath

    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.element = element;
            acceptDestinationInstance.newPath = newPath;
            acceptDestinationInstance.newName = newName;
            acceptDestinationInstance.overwrite = false;
        }
    }

    canAccept: canAcceptCheck
    acceptDestination: Qt.resolvedUrl("../pages/MoveLoadingPage.qml")
    acceptDestinationAction: PageStackAction.Replace
}
