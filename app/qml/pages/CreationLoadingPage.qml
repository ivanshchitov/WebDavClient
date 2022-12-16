// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    property string name
    property string path

    onStatusChanged: {
        if (root.status === PageStatus.Active)
            controller.initCreateFolderOperation();
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

        text: qsTr("Creating folder...")
    }

    FilesAndFoldersActionsController {
        id: controller

        onOperationError: {
            notification.summary = msgTitle.arg(name);
            notification.body = errorMessage;
            notification.publish();
            pageStack.pop();
        }
        onOperationSuccess: {
            notification.summary = msgTitle.arg(name);
            notification.body = "";
            notification.publish();
            pageStack.replace(Qt.resolvedUrl("FolderPage.qml"), {
                    "folderName": root.name,
                    "path": root.path + "/"
                });
        }

        path: root.path
        settings: appSettings
    }

    Notification {
        id: notification
    }
}
