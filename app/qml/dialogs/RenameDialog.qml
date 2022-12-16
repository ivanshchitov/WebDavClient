// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

EditFileOrFolderNameDialog {
    property var element
    property string newName
    property string pathToFolder

    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.name = renameController.getName(element);
            acceptDestinationInstance.newName = newName;
            acceptDestinationInstance.pathToFolder = pathToFolder;
        }
    }

    canAccept: canAcceptCheck
    acceptDestination: Qt.resolvedUrl("../pages/RenameLoadingPage.qml")
    acceptDestinationAction: PageStackAction.Push

    RenameController {
        id: renameController
    }
}
