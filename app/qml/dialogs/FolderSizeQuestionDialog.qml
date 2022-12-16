// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Dialog {
    id: root

    property int actionFlag
    property var folderElement

    onStatusChanged: {
        if (status === PageStatus.Active)
            controller.runFolderSizeCalculatingOperation(controller.getName(root.folderElement));
    }

    canAccept: !stumb.visible

    acceptDestination: folderPickerPage
    acceptDestinationAction: PageStackAction.Push

    DialogHeader {
        id: dialogHeader

        anchors.bottomMargin: Theme.paddingLarge

        acceptText: {
            switch (root.actionFlag) {
            case (ActionFlags.Download):
                return qsTr("Download");
            case (ActionFlags.MoveToDevice):
                return qsTr("Move");
            }
        }
        cancelText: qsTr("Cancel")
    }

    Label {
        id: sizeLabel

        anchors {
            top: dialogHeader.bottom
            left: parent.left
            leftMargin: Theme.paddingMedium
            right: parent.right
            rightMargin: Theme.paddingMedium
        }

        visible: !stumb.visible
        width: parent.width
        height: implicitHeight
        wrapMode: Text.WordWrap
    }

    Rectangle {
        id: stumb

        anchors.fill: parent

        opacity: Theme.opacityFaint
        color: Theme.darkPrimaryColor

        TouchBlocker {
            anchors.fill: parent
        }
    }

    BusyIndicator {
        id: stumbIndicator

        anchors.centerIn: parent

        size: BusyIndicatorSize.Large
        running: visible
        visible: stumb.visible
    }

    Label {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: stumbIndicator.bottom
            topMargin: Theme.paddingMedium
        }

        visible: stumb.visible

        text: qsTr("Calculating folder size...")
    }

    FolderSizeQuestionDialogController {
        id: controller

        onOperationError: {
            notification.summary = msgTitle.arg(controller.getName(root.folderElement));
            notification.body = errorMessage;
            notification.publish();
            pageStack.pop(findPageByObjectName("FolderPage"));
        }
        onOperationSuccess: {
            var name = controller.getName(root.folderElement);
            var formatFolderSize = Format.formatFileSize(controller.folderSize);
            sizeLabel.text = qsTr("%1 directory's size is %2. Start downloading?").arg(name).arg(formatFolderSize);
            stumb.visible = false;
        }

        path: controller.getPath(root.folderElement)
        settings: appSettings
    }

    Component {
        id: folderPickerPage

        FolderPickerPage {
            onSelectedPathChanged: pageStack.push(Qt.resolvedUrl("../pages/" + controller.getPathToPageByAction(actionFlag)), {
                    "element": root.folderElement,
                    "devicePath": selectedPath
                })

            dialogTitle: qsTr("Choose folder")
            title: qsTr("Choose folder")
        }
    }

    Notification {
        id: notification
    }
}
