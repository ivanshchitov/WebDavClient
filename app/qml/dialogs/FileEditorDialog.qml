// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    // hack to make Page appear as Dialog when using DialogHeader
    property int __silica_dialog
    property Item _dialogHeader
    property bool canAccept: root.state === "editing" && editorController.isEdited

    property string path
    property string prevState

    function accept() {
        if (canAccept) {
            changeState("checkingForConflicts");
            editorController.checkForConflict(path);
        }
    }

    function reject() {
        pageStack.pop();
    }

    function uploadFile(fileAbsolutiPath, cloudPath) {
        changeState("uploading");
        uploadController.initUploadFileOperation(fileAbsolutiPath, cloudPath, PoliticsFlags.Overwrite);
    }

    function changeState(newState) {
        prevState = state;
        state = newState;
    }

    Component.onCompleted: {
        changeState("downloading");
        editorController.openFile(path);
    }

    forwardNavigation: canAccept
    state: "downloading"
    states: [
        State {
            name: "downloading"
            PropertyChanges {
                target: downloadStatusColumn
                visible: true
            }
            PropertyChanges {
                target: editorStatusColumn
                visible: false
            }
            PropertyChanges {
                target: indeterminateWaitColumn
                visible: false
            }
            PropertyChanges {
                target: conflictResolutionColumn
                visible: false
            }
            PropertyChanges {
                target: errorColumn
                visible: false
            }
        },
        State {
            name: "editing"
            PropertyChanges {
                target: downloadStatusColumn
                visible: false
            }
            PropertyChanges {
                target: editorStatusColumn
                visible: true
            }
            PropertyChanges {
                target: indeterminateWaitColumn
                visible: false
            }
            PropertyChanges {
                target: conflictResolutionColumn
                visible: false
            }
            PropertyChanges {
                target: errorColumn
                visible: false
            }
        },
        State {
            name: "checkingForConflicts"
            PropertyChanges {
                target: downloadStatusColumn
                visible: false
            }
            PropertyChanges {
                target: editorStatusColumn
                visible: false
            }
            PropertyChanges {
                target: indeterminateWaitColumn
                visible: true
            }
            PropertyChanges {
                target: conflictResolutionColumn
                visible: false
            }
            PropertyChanges {
                target: errorColumn
                visible: false
            }
        },
        State {
            name: "uploading"
            PropertyChanges {
                target: downloadStatusColumn
                visible: false
            }
            PropertyChanges {
                target: editorStatusColumn
                visible: false
            }
            PropertyChanges {
                target: indeterminateWaitColumn
                visible: true
            }
            PropertyChanges {
                target: conflictResolutionColumn
                visible: false
            }
            PropertyChanges {
                target: errorColumn
                visible: false
            }
        },
        State {
            name: "conflictResolution"
            PropertyChanges {
                target: downloadStatusColumn
                visible: false
            }
            PropertyChanges {
                target: editorStatusColumn
                visible: false
            }
            PropertyChanges {
                target: indeterminateWaitColumn
                visible: false
            }
            PropertyChanges {
                target: conflictResolutionColumn
                visible: true
            }
            PropertyChanges {
                target: errorColumn
                visible: false
            }
        },
        State {
            name: "error"
            PropertyChanges {
                target: downloadStatusColumn
                visible: false
            }
            PropertyChanges {
                target: editorStatusColumn
                visible: false
            }
            PropertyChanges {
                target: indeterminateWaitColumn
                visible: false
            }
            PropertyChanges {
                target: conflictResolutionColumn
                visible: false
            }
            PropertyChanges {
                target: errorColumn
                visible: true
            }
        }
    ]

    DialogHeader {
        id: dialogHeader

        width: parent.width

        acceptText: qsTr("Save")
        cancelText: qsTr("Cancel")
    }

    Column {
        id: downloadStatusColumn

        anchors.centerIn: parent
        width: parent.width

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Downloading file...")
        }
        ProgressBar {
            id: progressBar

            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
        }
    }

    Column {
        id: editorStatusColumn

        anchors.centerIn: parent
        width: parent.width

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            maximumLineCount: 2
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            text: qsTr("Please confirm when you've finished editing file")
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            text: path
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            text: qsTr("Last modified: %1").arg(Qt.formatDateTime(editorController.lastModified, "hh:mm:ss"))
        }
    }

    Column {
        id: indeterminateWaitColumn

        anchors.centerIn: parent
        width: parent.width

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Please wait...")
        }
        ProgressBar {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            indeterminate: true
        }
    }

    Column {
        id: conflictResolutionColumn

        anchors {
            left: parent.left
            right: parent.right
            centerIn: parent
            margins: Theme.paddingMedium
        }

        width: parent.width
        spacing: Theme.paddingMedium

        Label {
            id: conflictDescription

            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            maximumLineCount: 5
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        Button {
            text: qsTr("Replace local file")
            anchors.horizontalCenter: parent.horizontalCenter
            width: Theme.buttonWidthLarge
            onClicked: {
                changeState("downloading");
                editorController.openFile(path);
            }
        }

        Button {
            text: qsTr("Replace remote file")
            anchors.horizontalCenter: parent.horizontalCenter
            width: Theme.buttonWidthLarge
            onClicked: {
                uploadFile(editorController.filePath, root.path);
            }
        }

        Button {
            text: qsTr("Save local copy")
            anchors.horizontalCenter: parent.horizontalCenter
            width: Theme.buttonWidthLarge
            onClicked: {
                var picker = pageStack.push("Sailfish/Pickers/FolderPickerDialog", {
                        "title": qsTr("Save to")
                    });
                picker.accepted.connect(function () {
                        editorController.saveTo(picker.selectedPath);
                        pageStack.pop(pageStack.previousPage(root));
                    });
            }
        }
    }

    Column {
        id: errorColumn

        anchors.centerIn: parent
        width: parent.width

        Label {
            id: errorLabel

            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            verticalAlignment: Text.AlignVCenter
        }
        Button {
            text: qsTr("Retry")
            anchors.horizontalCenter: parent.horizontalCenter
            width: Theme.buttonWidthLarge
            onClicked: {
                switch (root.prevState) {
                case "downloading":
                    changeState("downloading");
                    editorController.openFile(path);
                    break;
                case "checkingForConflicts":
                    changeState("checkingForConflicts");
                    editorController.checkForConflict(path);
                    break;
                case "uploading":
                    uploadFile(editorController.filePath, root.path);
                    break;
                }
            }
        }

        Button {
            text: qsTr("Save local copy")
            anchors.horizontalCenter: parent.horizontalCenter
            width: Theme.buttonWidthLarge
            onClicked: {
                var picker = pageStack.push("Sailfish/Pickers/FolderPickerDialog", {
                        "title": qsTr("Save to")
                    });
                picker.accepted.connect(function () {
                        editorController.saveTo(picker.selectedPath);
                        pageStack.pop(pageStack.previousPage(root));
                    });
            }
        }
    }

    FileEditorController {
        id: editorController

        onDownloadProgressChanged: {
            progressBar.value = percent;
            if (percent >= 1)
                changeState("editing");
        }
        onConflictCheckDone: {
            if (status) {
                conflictDescription.text = reason;
                changeState("conflictResolution");
            } else {
                uploadFile(editorController.filePath, root.path);
            }
        }
        onErrorOccured: {
            errorLabel.text = errorMessage;
            changeState("error");
        }

        settings: appSettings
    }

    FilesAndFoldersActionsController {
        id: uploadController

        onOperationError: {
            errorLabel.text = errorMessage;
            changeState("error");
        }
        onOperationSuccess: {
            notification.summary = msgTitle.arg(root.path.split("/").pop());
            notification.body = "";
            notification.publish();
            pageStack.pop(pageStack.previousPage(root));
        }

        path: ""
        settings: appSettings
    }

    Notification {
        id: notification
    }
}
