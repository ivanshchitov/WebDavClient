// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0
import "../components"

Page {
    id: root

    property int currentAction
    property var currentElement
    property string folderName: qsTr("All files")
    property ElementListSortFilterModel tableModel: ElementListSortFilterModel {
        sourceModel: controller.entitiesModel
    }

    property alias path: controller.path

    objectName: "FolderPage"

    onStatusChanged: {
        if (status === PageStatus.Activating)
            controller.initLoadFilesAndFoldersOperation();
    }

    FolderPageController {
        id: controller

        onConnectionInProgressChanged: pageHeader.busy = controller.connectionInProgress
        onFilesAndFoldersLoadingFinished: {
            if (updateStumb.visible)
                updateStumb.visible = false;
        }
        onOperationSuccess: {
            notification.summary = msgTitle;
            notification.body = "";
            notification.publish();
        }
        onOperationError: {
            notification.summary = msgTitle;
            notification.body = errorMessage;
            notification.publish();
        }

        path: "/"
        settings: appSettings
    }

    SilicaFlickable {
        id: contentContainer

        anchors.fill: parent

        Loader {
            id: pageMenuLoader

            active: !controller.connectionInProgress
            sourceComponent: FolderPagePullMenu {
            }
        }

        RemotePageHeader {
            id: pageHeader

            objectName: "pageHeader"
            title: root.folderName
            display: false
        }

        SilicaListView {
            id: documentsView

            anchors {
                left: parent.left
                right: parent.right
                top: pageHeader.bottom
                bottom: parent.bottom
            }

            clip: true
            model: tableModel

            delegate: FileDelegate {
                onClicked: {
                    if (model.is_file)
                        return;
                    pageStack.push(Qt.resolvedUrl("%1.qml".arg(root.objectName)), {
                            "folderName": model.name,
                            "path": root.path + model.name + "/"
                        });
                }

                timestamp: Format.formatDate(model.timestamp, Format.DateLong)
                name: model.name
                size: Format.formatFileSize(model.size)
                menu: RemoteContextMenu {
                    isFile: is_file
                }
                path: model.path
                busy: model.busy
            }

            VerticalScrollDecorator {
            }
        }

        EmptyFolderStub {
            id: stub

            anchors {
                top: pageHeader.bottom
                left: parent.left
                right: parent.right
            }

            title: qsTr("This folder is empty")
            visible: documentsView.count <= 0 && !pageHeader.busy
        }

        Component {
            id: filePickerPage

            FilePickerPage {
                onSelectedContentPropertiesChanged: pageStack.push(Qt.resolvedUrl("UploadLoadingPage.qml"), {
                        "isFile": true,
                        "model": controller.entitiesModel,
                        "path": root.path,
                        "name": selectedContentProperties.fileName,
                        "devicePath": selectedContentProperties.filePath
                    })

                title: qsTr("Choose file")
            }
        }

        Component {
            id: folderPickerPage

            FolderPickerPage {
                onSelectedPathChanged: {
                    var properties;
                    if (root.currentAction === ActionFlags.Upload)
                        properties = {
                            "isFile": false,
                            "model": controller.entitiesModel,
                            "path": root.path,
                            "name": selectedPath.split("/").pop(),
                            "devicePath": selectedPath
                        };
                    else
                        properties = {
                            "element": root.currentElement,
                            "devicePath": selectedPath
                        };
                    pageStack.push(Qt.resolvedUrl(controller.getPathToPageByAction(root.currentAction)), properties);
                }

                dialogTitle: qsTr("Choose folder")
                title: qsTr("Choose folder")
            }
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

        size: BusyIndicatorSize.Large
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

        text: qsTr("Refreshing folder...")
    }

    Notification {
        id: notification
    }
}
