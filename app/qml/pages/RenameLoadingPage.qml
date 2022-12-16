// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    property string name
    property string newName
    property string pathToFolder

    onStatusChanged: {
        if (root.status === PageStatus.Active)
            controller.renameFilesOrFolders(root.pathToFolder, root.name, root.newName);
    }

    backNavigation: false

    BusyIndicator {
        id: loadingIndication

        anchors.centerIn: parent

        size: BusyIndicatorSize.Large
        running: true
    }

    Label {
        id: loadingLabel

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: loadingIndication.bottom
            topMargin: Theme.paddingMedium
        }

        text: qsTr("Renaming...")
    }

    FilesAndFoldersActionsController {
        id: controller

        onOperationSuccess: {
            notification.summary = msgTitle;
            notification.body = "";
            notification.publish();
            pageStack.pop(findPageByObjectName("FolderPage"));
        }
        onOperationError: {
            pageStack.previousPage().error = errorMessage;
            pageStack.previousPage().visibleError = true;
            pageStack.pop();
        }

        path: "/"
        settings: appSettings
    }

    Notification {
        id: notification
    }
}
