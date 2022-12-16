// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    property int port
    property int service
    property string serverAddress
    property string username
    property string password
    property var token

    onStatusChanged: {
        if (status === PageStatus.Active)
            switch (root.service) {
            case ServicesFlags.NextCloud:
                controller.startConnection(serverAddress, username, password, ServicesFlags.NextCloud);
                break;
            case ServicesFlags.YandexDisk:
                controller.startConnection(token);
                break;
            case ServicesFlags.Communigate:
                controller.startConnection(serverAddress, username, password, ServicesFlags.Communigate, port);
                break;
            case ServicesFlags.WebDavServer:
                controller.startConnection(serverAddress, username, password, ServicesFlags.WebDavServer);
                break;
            }
    }

    backNavigation: false

    BusyIndicator {
        id: loadingIndication

        anchors.centerIn: parent

        size: BusyIndicatorSize.Large
        running: true
    }

    Label {
        id: loadingLabel

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: loadingIndication.bottom
            topMargin: Theme.paddingMedium
        }

        text: qsTr("Authorization...")
    }

    ConnectionController {
        id: controller

        onConnectionResultChanged: {
            notification.summary = qsTr("Authorization is successful");
            notification.publish();
            pageStack.replaceAbove(findPageByObjectName("AccountSelectionPage"), Qt.resolvedUrl("FolderPage.qml"));
        }

        onConnectionErrorChanged: {
            notification.summary = qsTr("Authorization error");
            notification.body = controller.errorMessage();
            notification.publish();
            pageStack.pop();
        }

        onInvalidToken: {
            pageStack.replace(Qt.resolvedUrl("../pages/OAuthPage.qml"));
        }

        settings: appSettings
        dao: appDao
    }

    Notification {
        id: notification
    }
}
