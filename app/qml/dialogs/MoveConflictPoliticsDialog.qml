// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ConflictPoliticsDialog {
    property var element
    property string newPath

    onAccepted: {
        if (acceptDestination) {
            switch (currentPolitic) {
            case (PoliticsFlags.Overwrite):
                acceptDestinationInstance.element = element;
                acceptDestinationInstance.newPath = newPath;
                acceptDestinationInstance.overwrite = true;
                break;
            case (PoliticsFlags.Rename):
                acceptDestinationInstance.element = element;
                acceptDestinationInstance.newPath = newPath;
                break;
            }
        }
    }

    acceptDestination: {
        switch (currentPolitic) {
        case (PoliticsFlags.Overwrite):
            Qt.resolvedUrl("../pages/MoveLoadingPage.qml");
            break;
        case (PoliticsFlags.Rename):
            Qt.resolvedUrl("RenameMovingFileOrFolderDialog.qml");
            break;
        }
    }
}
