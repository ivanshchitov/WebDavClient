// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ConflictPoliticsDialog {
    property FileAndFolderListModel model
    property string name
    property string path
    property string devicePath
    property bool isFile

    onAccepted: {
        if (acceptDestination) {
            switch (currentPolitic) {
            case (PoliticsFlags.Overwrite):
                acceptDestinationInstance.model = model;
                acceptDestinationInstance.path = path;
                acceptDestinationInstance.name = name;
                acceptDestinationInstance.devicePath = devicePath;
                acceptDestinationInstance.flag = PoliticsFlags.Overwrite;
                acceptDestinationInstance.isFile = isFile;
                break;
            case (PoliticsFlags.Rename):
                acceptDestinationInstance.model = model;
                acceptDestinationInstance.path = path.substring(0, path.lastIndexOf("/") + 1);
                acceptDestinationInstance.conflictName = name.substring(0, name.lastIndexOf('.'));
                acceptDestinationInstance.extension = "." + name.split(".").pop();
                acceptDestinationInstance.devicePath = devicePath;
                acceptDestinationInstance.isFile = isFile;
                break;
            }
        }
    }

    acceptDestination: {
        switch (currentPolitic) {
        case (PoliticsFlags.Overwrite):
            Qt.resolvedUrl("../pages/UploadLoadingPage.qml");
            break;
        case (PoliticsFlags.Rename):
            Qt.resolvedUrl("RenameUploadingFileDialog.qml");
            break;
        }
    }
}
