// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

EditFileOrFolderNameDialog {
    id: dialog

    property var element
    property string devicePath
    property bool moveToDevice

    property string newName

    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.element = element;
            acceptDestinationInstance.devicePath = devicePath;
            acceptDestinationInstance.newName = newName;
            acceptDestinationInstance.overwrite = false;
        }
    }

    canAccept: canAcceptCheck
    acceptDestination: dialog.moveToDevice ? Qt.resolvedUrl("../pages/MoveToDeviceLoadingPage.qml") : Qt.resolvedUrl("../pages/DownloadLoadingPage.qml")
    acceptDestinationAction: PageStackAction.Replace
}
