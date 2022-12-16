// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Dialog {
    id: root

    property FileAndFolderListModel model
    property string conflictName
    property string extension
    property string name
    property string path
    property string devicePath
    property bool isFile

    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.isFile = true;
            acceptDestinationInstance.model = model;
            acceptDestinationInstance.path = path;
            acceptDestinationInstance.name = renameTextField.text.trim() + extension;
            acceptDestinationInstance.devicePath = devicePath;
            acceptDestinationInstance.isFile = isFile;
        }
    }

    canAccept: root.conflictName === renameTextField.text ? false : renameTextField.acceptableInput

    acceptDestination: Qt.resolvedUrl("../pages/UploadLoadingPage.qml")
    acceptDestinationAction: PageStackAction.Replace

    DialogHeader {
        id: dialogHeader

        anchors.bottomMargin: Theme.paddingLarge

        acceptText: qsTr("Rename")
        cancelText: qsTr("Cancel")
    }

    TextField {
        id: renameTextField

        anchors {
            top: dialogHeader.bottom
            left: parent.left
            right: parent.right
            topMargin: Theme.paddingMedium
        }

        focus: true
        text: root.conflictName
        label: qsTr("Enter new file name")
        placeholderText: qsTr("File name")

        validator: RegExpValidator {
            regExp: /(?!\.)[^|\\\/<>:\?\*]{1,246}/
        }

        EnterKey.enabled: root.conflictName === renameTextField.text ? false : acceptableInput
        EnterKey.iconSource: "image://theme/icon-m-enter-accept"
        EnterKey.onClicked: root.accept()
    }

    Label {
        id: errorMessage

        anchors {
            verticalCenter: parent.verticalCenter
            top: renameTextField.bottom
            left: parent.left
            right: parent.right
            margins: Theme.horizontalPageMargin
        }

        horizontalAlignment: Text.AlignHCenter
        visible: root.conflictName === renameTextField.text

        text: qsTr("New name cannot match the previous one!")
        wrapMode: Text.Wrap
        font.pixelSize: Theme.fontSizeSmall
        color: "red"
    }
}
