// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0
import "../components"

Dialog {
    id: authorizationPage

    property bool keyCheck: serverAddressTextField.text && usernameTextField.text && passwordField.text
    property bool allowInput: true

    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.service = ServicesFlags.Communigate;
            if (portTextField.length > 0)
                acceptDestinationInstance.port = portTextField.text;
            acceptDestinationInstance.serverAddress = serverAddressTextField.text;
            acceptDestinationInstance.username = usernameTextField.text;
            acceptDestinationInstance.password = passwordField.text;
        }
    }

    canAccept: authorizationPage.keyCheck && authorizationPage.allowInput
    acceptDestination: Qt.resolvedUrl("../pages/AuthorizationLoadingPage.qml")
    acceptDestinationAction: PageStackAction.Push

    DialogHeader {
        id: authorizationHeader

        objectName: "dialogHeader"
        title: qsTr("Authorization")
        acceptText: qsTr("Continue")
        cancelText: qsTr("Cancel")
    }

    Column {
        id: column

        anchors {
            top: authorizationHeader.bottom
            left: parent.left
            right: parent.right
        }

        spacing: Theme.paddingMedium

        TextField {
            id: serverAddressTextField

            anchors {
                left: parent.left
                right: parent.right
            }

            label: qsTr("Enter your Communigate server address")
            placeholderText: qsTr("Communigate server address")
            inputMethodHints: Qt.ImhUrlCharactersOnly
            readOnly: !authorizationPage.allowInput

            EnterKey.onClicked: portTextField.focus = true
        }

        TextField {
            id: portTextField

            anchors {
                left: parent.left
                right: parent.right
            }

            label: qsTr("Enter the port to connect")
            placeholderText: qsTr("Connection port")
            inputMethodHints: Qt.ImhDigitsOnly
            readOnly: !authorizationPage.allowInput
            validator: RegExpValidator {
                regExp: /^(\s*|\d+)$/
            }

            EnterKey.onClicked: usernameTextField.focus = true
        }

        TextField {
            id: usernameTextField

            anchors {
                left: parent.left
                right: parent.right
            }

            label: qsTr("Enter your username")
            placeholderText: qsTr("Username")
            readOnly: !authorizationPage.allowInput
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase

            EnterKey.onClicked: passwordField.focus = true
        }

        PasswordField {
            id: passwordField

            anchors {
                left: parent.left
                right: parent.right
            }

            label: qsTr("Enter your password")
            placeholderText: qsTr("Password")
            readOnly: !authorizationPage.allowInput

            EnterKey.enabled: authorizationPage.canAccept
            EnterKey.onClicked: {
                if (authorizationPage.canAccept)
                    authorizationPage.accept();
            }
        }
    }
}
