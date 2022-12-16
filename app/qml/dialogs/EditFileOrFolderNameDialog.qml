// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Dialog {
    id: root

    property alias error: errorMessage.text
    property alias visibleError: errorMessage.visible
    property alias canAcceptCheck: textField.acceptableInput

    DialogHeader {
        id: rootHeader

        acceptText: qsTr("Rename")
        cancelText: qsTr("Cancel")
    }

    TextField {
        id: textField

        onTextChanged: root.newName = text

        anchors {
            top: rootHeader.bottom
            left: parent.left
            right: parent.right
            topMargin: Theme.paddingLarge
        }

        text: controller.getName(root.element)

        label: controller.getIsFile(root.element) ? qsTr("Enter new file name") : qsTr("Enter new folder name")
        placeholderText: controller.getIsFile(root.element) ? qsTr("File name") : qsTr("Folder name")

        validator: RegExpValidator {
            regExp: controller.checkNewName()
        }

        EnterKey.enabled: acceptableInput
        EnterKey.iconSource: "image://theme/icon-m-enter-accept"
        EnterKey.onClicked: root.accept()
    }

    Label {
        id: errorMessage

        anchors {
            verticalCenter: parent.verticalCenter
            top: textField.bottom
            left: parent.left
            right: parent.right
            margins: Theme.horizontalPageMargin
        }

        horizontalAlignment: Text.AlignHCenter
        visible: false

        wrapMode: Text.Wrap
        font.pixelSize: Theme.fontSizeSmall
        color: "red"
    }

    RenameController {
        id: controller
    }
}
