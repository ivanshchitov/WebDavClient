// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    property var element
    property string newPath
    property bool overwrite
    property string newName

    onStatusChanged: {
        if (root.status === PageStatus.Activating)
            controllerMove.initLoadFilesAndFoldersOperation();
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

        text: qsTr("Moving...")
    }

    FilesAndFoldersActionsController {
        id: controller

        onConflictOccured: {
            pageStack.completeAnimation();
            pageStack.replaceAbove(findPageByObjectName("FolderPage"), Qt.resolvedUrl("../dialogs/MoveConflictPoliticsDialog.qml"), {
                    "element": root.element,
                    "newPath": root.newPath
                });
        }
        onOperationSuccess: {
            notification.summary = msgTitle;
            notification.body = "";
            notification.publish();
            pageStack.pop(findPageByObjectName("FolderPage"));
        }
        onOperationError: {
            notification.summary = msgTitle;
            notification.body = errorMessage;
            notification.publish();
            pageStack.pop(findPageByObjectName("FolderPage"));
        }

        path: root.newPath
        settings: appSettings
    }

    SelectFolderDialogController {
        id: controllerMove

        onFilesAndFoldersLoadingFinished: {
            controller.setEntitiesModel(controllerMove.entitiesModel);
            controller.moveFilesOrFolders(root.element, root.newPath, root.overwrite, root.newName);
        }
        path: root.newPath
        settings: appSettings
    }

    Notification {
        id: notification
    }
}
