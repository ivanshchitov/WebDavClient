// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    property bool isFile
    property int flag: PoliticsFlags.Rename
    property FileAndFolderListModel model
    property string name
    property string path
    property string devicePath

    onStatusChanged: {
        if (root.status === PageStatus.Active) {
            controller.setEntitiesModel(model);
            if (isFile)
                controller.initUploadFileOperation(devicePath, name, flag);
            else
                controller.runUploadFolderOperation(devicePath);
        }
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

        text: isFile ? qsTr("Uploading file...") : qsTr("Uploading folder...") + " (1/2)\n" + qsTr("Creating folders...")
    }

    FilesAndFoldersActionsController {
        id: controller

        onConflictOccured: pageStack.replaceAbove(findPageByObjectName("FolderPage"), Qt.resolvedUrl("../dialogs/UploadConflictPoliticsDialog.qml"), {
                "path": root.path,
                "name": root.name,
                "devicePath": root.devicePath,
                "model": model,
                "isFile": root.isFile
            })
        onOperationError: {
            notification.summary = msgTitle.arg(name);
            notification.body = errorMessage;
            notification.publish();
            pageStack.pop(findPageByObjectName("FolderPage"));
        }
        onOperationSuccess: {
            notification.summary = msgTitle.arg(name);
            notification.body = "";
            notification.publish();
            if (isFile)
                pageStack.pop(findPageByObjectName("FolderPage"));
            else
                pageStack.replaceAbove(findPageByObjectName("FolderPage"), Qt.resolvedUrl("FolderPage.qml"), {
                        "folderName": root.name,
                        "path": root.path + root.name + "/"
                    });
        }
        onOperationProgressChanged: loadingLabel.text = qsTr("Uploading folder...") + " (2/2)\n" + qsTr("Uploading files...")

        path: root.path
        settings: appSettings
        entitiesModel: model
    }

    Notification {
        id: notification
    }
}
