// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    property string devicePath: StandardPaths.download
    property var element
    property bool overwrite: false
    property string newName

    onStatusChanged: {
        if (root.status === PageStatus.Active)
            if (controller.getIsFile(root.element))
                controller.runDownloadFileOperation(controller.getName(root.element), root.devicePath, root.overwrite, root.newName);
            else
                controller.runDownloadFolderOperation(controller.getName(root.element), root.devicePath, root.overwrite, root.newName);
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

        text: controller.getIsFile(root.element) ? qsTr("Downloading file...") : qsTr("Downloading folder...") + " (1/2)\n" + qsTr("Creating folders...")
    }

    FilesAndFoldersActionsController {
        id: controller

        onConflictOccured: {
            pageStack.completeAnimation();
            pageStack.replaceAbove(findPageByObjectName("FolderPage"), Qt.resolvedUrl("../dialogs/DownloadConflictPoliticsDialog.qml"), {
                    "element": root.element,
                    "devicePath": root.devicePath,
                    "moveToDevice": false
                });
        }
        onOperationError: {
            notification.summary = msgTitle.arg(controller.getName(root.element));
            notification.body = errorMessage;
            notification.publish();
            pageStack.pop(findPageByObjectName("FolderPage"));
        }
        onOperationSuccess: {
            notification.summary = msgTitle.arg(controller.getName(root.element));
            notification.body = "";
            notification.publish();
            pageStack.pop(findPageByObjectName("FolderPage"));
        }
        onOperationProgressChanged: loadingLabel.text = qsTr("Downloading folder...") + " (2/2)\n" + qsTr("Downloading files...")

        path: controller.getPath(root.element)
        settings: appSettings
    }

    Notification {
        id: notification
    }
}
