// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ContextMenu {
    property bool isFile

    MenuItem {
        onClicked: {
            pageStack.push(Qt.resolvedUrl("../dialogs/RenameDialog.qml"), {
                    "element": controller.getCurrentItem(model.path),
                    "pathToFolder": root.path
                });
        }

        text: qsTr("Rename")
    }

    MenuItem {
        onClicked: {
            pageStack.push(Qt.resolvedUrl("../dialogs/MoveSelectFolderDialog.qml"), {
                    "element": controller.getCurrentItem(model.path)
                });
        }

        text: qsTr("Move")
    }

    MenuItem {
        onClicked: {
            pageStack.push(Qt.resolvedUrl("../dialogs/FileEditorDialog.qml"), {
                    "path": path
                });
        }

        text: qsTr("Edit")
        visible: isFile
    }

    MenuItem {
        onClicked: {
            remorseDelete(function () {
                    controller.deleteFilesOrFolders(path, isFile);
                });
        }

        text: qsTr("Delete")
    }

    MenuItem {
        onClicked: {
            if (isFile) {
                currentAction = ActionFlags.Download;
                currentElement = controller.getCurrentItem(model.path);
                pageStack.push(folderPickerPage);
            } else
                pageStack.push(Qt.resolvedUrl("../dialogs/FolderSizeQuestionDialog.qml"), {
                        "folderElement": controller.getCurrentItem(model.path),
                        "actionFlag": ActionFlags.Download
                    });
        }

        text: qsTr("Download")
    }

    MenuItem {
        onClicked: {
            if (isFile) {
                currentAction = ActionFlags.MoveToDevice;
                currentElement = controller.getCurrentItem(model.path);
                pageStack.push(folderPickerPage);
            } else
                pageStack.push(Qt.resolvedUrl("../dialogs/FolderSizeQuestionDialog.qml"), {
                        "folderElement": controller.getCurrentItem(model.path),
                        "actionFlag": ActionFlags.MoveToDevice
                    });
        }

        text: qsTr("Move to device")
    }
}
