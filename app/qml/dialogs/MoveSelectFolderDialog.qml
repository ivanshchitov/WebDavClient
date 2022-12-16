// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0
import "../components"

Dialog {
    id: root

    property var element
    property string folderName: qsTr("All files")
    property ElementListSortFilterModel tableModel: ElementListSortFilterModel {
        sourceModel: controller.entitiesModel
    }

    property alias path: controller.path

    acceptDestination: Qt.resolvedUrl("../pages/MoveLoadingPage.qml")
    acceptDestinationAction: PageStackAction.Replace

    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.element = root.element;
            acceptDestinationInstance.newPath = root.path;
            acceptDestinationInstance.overwrite = false;
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            updateStumb.visible = true;
            controller.initLoadFilesAndFoldersOperation();
        }
    }

    SelectFolderDialogController {
        id: controller

        onFilesAndFoldersLoadingFinished: {
            if (updateStumb.visible)
                updateStumb.visible = false;
        }
        path: "/"
        settings: appSettings
    }

    SilicaFlickable {
        id: contentContainer

        anchors.fill: parent

        DialogHeader {
            id: dialogHeader

            defaultAcceptText: qsTr("Choose")
            defaultCancelText: qsTr("Cancel")
        }

        Label {
            id: labelHeader

            anchors {
                top: dialogHeader.bottom
                left: parent.left
                right: parent.right
                verticalCenter: parent.horizontalCenter
                leftMargin: Theme.paddingLarge
                rightMargin: Theme.paddingMedium
                bottomMargin: Theme.paddingSmall
                topMargin: Theme.paddingSmall
            }

            wrapMode: Text.Wrap
            width: parent.width
            height: Theme.itemSizeSmall
            font.pixelSize: Theme.fontSizeLarge
            color: Theme.highlightColor
            text: qsTr("Chosen directory:") + " " + root.folderName
        }

        BackgroundItem {
            id: returnButton

            onClicked: {
                updateStumb.visible = true;
                root.path = controller.returnParentPath(root.path);
                root.folderName = controller.returnParentName(root.path);
                controller.initLoadFilesAndFoldersOperation();
            }

            anchors {
                top: labelHeader.bottom
                left: parent.left
                right: parent.right
                bottomMargin: Theme.paddingMedium
            }

            width: parent.width
            height: Theme.itemSizeSmall
            highlighted: pressed
            visible: controller.abilityToReturn(root.path)

            Label {
                id: label

                anchors {
                    left: icon.right
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    leftMargin: Theme.paddingMedium
                    rightMargin: Theme.horizontalPageMargin
                }
                text: qsTr("Return to:") + " " + controller.returnParentName(controller.returnParentPath(root.path))
            }

            Icon {
                id: icon

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    leftMargin: Theme.horizontalPageMargin
                }
                source: "image://theme/icon-m-tab-return"
            }
        }

        SilicaListView {
            id: documentsView

            anchors {
                left: parent.left
                right: parent.right
                top: returnButton.visible ? returnButton.bottom : labelHeader.bottom
                bottom: parent.bottom
            }

            clip: true
            model: tableModel

            delegate: FileDelegate {
                onClicked: {
                    updateStumb.visible = true;
                    root.path = model.path;
                    root.folderName = model.name;
                    controller.initLoadFilesAndFoldersOperation();
                }

                enabled: controller.abilityToInteract(root.element, model.path, model.is_file)
                opacity: enabled ? 1.0 : Theme.opacityFaint

                timestamp: Format.formatDate(model.timestamp, Format.DateLong)
                name: model.name
                size: Format.formatFileSize(model.size)

                path: model.path
                busy: model.busy
            }

            VerticalScrollDecorator {
            }
        }

        EmptyFolderStub {
            id: stub

            anchors {
                top: returnButton.visible ? returnButton.bottom : labelHeader.bottom
                left: parent.left
                right: parent.right
            }

            title: qsTr("This folder is empty")
            visible: documentsView.count <= 0 && !updateStumb.visible
        }
    }

    Rectangle {
        id: updateStumb

        anchors.fill: parent

        opacity: Theme.opacityFaint
        color: Theme.darkPrimaryColor
        visible: false

        TouchBlocker {
            anchors.fill: parent
        }
    }

    BusyIndicator {
        id: updateIndicator

        anchors.centerIn: parent

        size: BusyIndicatorSize.Medium
        running: visible
        visible: updateStumb.visible
    }

    Label {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: updateIndicator.bottom
            topMargin: Theme.paddingMedium
        }

        visible: updateStumb.visible

        text: qsTr("Loading...")
    }
}
