// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0
import "../components"

Dialog {
    id: root

    property alias currentService: services.activeElement

    onAccepted: {
        switch (root.currentService) {
        case (ServicesFlags.NextCloud):
            pageStack.push(Qt.resolvedUrl("../dialogs/NextCloudAuthorizationDialog.qml"));
            break;
        case (ServicesFlags.YandexDisk):
            pageStack.push(Qt.resolvedUrl("../pages/OAuthPage.qml"));
            break;
        case (ServicesFlags.Communigate):
            pageStack.push(Qt.resolvedUrl("../dialogs/CommunigateAuthorizationDialog.qml"));
            break;
        case (ServicesFlags.WebDavServer):
            pageStack.push(Qt.resolvedUrl("../dialogs/WebDavServerAuthorizationDialog.qml"));
            break;
        }
    }

    objectName: "AuthorizationDialog"

    DialogHeader {
        id: dialogHeader

        objectName: "dialogHeader"
        title: qsTr("Service selection")
        acceptText: qsTr("Continue")
        cancelText: qsTr("Cancel")
    }

    SilicaFlickable {
        anchors {
            top: dialogHeader.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        contentHeight: contentColumn.height

        Column {
            id: contentColumn

            width: parent.width

            TextArea {
                width: contentColumn.width
                height: implicitHeight
                text: qsTr("Select the service to which you want to connect the account.")
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryHighlightColor
                readOnly: true
            }

            TextSwitchGroup {
                id: services

                activeElement: ServicesFlags.NextCloud
                width: contentColumn.width
                model: servicesModel
            }
        }

        VerticalScrollDecorator {
        }
    }

    ListModel {
        id: servicesModel

        ListElement {
            value: ServicesFlags.NextCloud
            textItem: qsTr("NextCloud")
            check: true
        }

        ListElement {
            value: ServicesFlags.YandexDisk
            textItem: qsTr("YandexDisk")
            check: false
        }

        ListElement {
            value: ServicesFlags.Communigate
            textItem: qsTr("Communigate")
            check: false
        }

        ListElement {
            value: ServicesFlags.WebDavServer
            textItem: qsTr("WebDAV server")
            check: false
        }
    }
}
