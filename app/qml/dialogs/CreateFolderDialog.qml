// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: root

    property string path

    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.path = path + creationTextField.text.trim();
            acceptDestinationInstance.name = creationTextField.text.trim();
        }
    }

    canAccept: creationTextField.acceptableInput

    acceptDestination: Qt.resolvedUrl("../pages/CreationLoadingPage.qml")
    acceptDestinationAction: PageStackAction.Replace

    DialogHeader {
        id: dialogHeader

        anchors.bottomMargin: Theme.paddingLarge

        acceptText: qsTr("Create")
        cancelText: qsTr("Cancel")
    }

    TextField {
        id: creationTextField

        anchors {
            top: dialogHeader.bottom
            left: parent.left
            right: parent.right
            topMargin: Theme.paddingLarge
        }

        focus: true
        label: qsTr("Enter folder name")
        placeholderText: qsTr("Folder name")

        validator: RegExpValidator {
            regExp: /(?!\.)[^|\\\/<>:\?\*]{1,250}/
        }

        EnterKey.enabled: acceptableInput
        EnterKey.iconSource: "image://theme/icon-m-enter-accept"
        EnterKey.onClicked: root.accept()
    }
}
