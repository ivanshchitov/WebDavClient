// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ConflictPoliticsDialog {
    id: dialog

    property var element
    property string devicePath
    property bool moveToDevice

    onAccepted: {
        if (acceptDestination) {
            switch (currentPolitic) {
            case (PoliticsFlags.Overwrite):
                acceptDestinationInstance.element = element;
                acceptDestinationInstance.devicePath = devicePath;
                acceptDestinationInstance.overwrite = true;
                break;
            case (PoliticsFlags.Rename):
                acceptDestinationInstance.element = element;
                acceptDestinationInstance.devicePath = devicePath;
                acceptDestinationInstance.moveToDevice = moveToDevice;
                break;
            }
        }
    }

    acceptDestination: {
        switch (currentPolitic) {
        case (PoliticsFlags.Overwrite):
            dialog.moveToDevice ? Qt.resolvedUrl("../pages/MoveToDeviceLoadingPage.qml") : Qt.resolvedUrl("../pages/DownloadLoadingPage.qml");
            break;
        case (PoliticsFlags.Rename):
            Qt.resolvedUrl("RenameDownloadFileOrFolderDialog.qml");
            break;
        }
    }
}
