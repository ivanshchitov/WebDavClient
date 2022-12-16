// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.WebView 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

Page {
    id: root

    property string rootClientId: clientId
    property string rootClientSecret: clientSecret
    property string url: "https://oauth.yandex.ru/authorize?response_type=code&client_id=%1&scope=yadisk:disk".arg(clientId)

    objectName: "oauthPage"

    WebView {
        property bool authorized: false

        privateMode: true
        url: root.url
        anchors.fill: parent
        onUrlChanged: function () {
            var parameters = UrlHelper.parseUrlParameters(url);
            if (parameters["code"] && !authorized) {
                authorized = true;
                oauth.getAccessToken(parameters["code"]);
            } else if (parameters["error"]) {
                notification.summary = qsTr("Authorization error");
                notification.body = parameters["error_description"];
                notification.publish();
                pageStack.pop();
            }
        }
    }

    OAuthProvider {
        id: oauth
        portalUrl: "https://oauth.yandex.ru/"
        clientId: rootClientId
        clientSecret: rootClientSecret
        onTokenChanged: {
            if (token.accessToken) {
                pageStack.replace(Qt.resolvedUrl("AuthorizationLoadingPage.qml"), {
                        "service": ServicesFlags.YandexDisk,
                        "token": token
                    });
            }
        }
        onErrorOccured: {
            notification.summary = qsTr("Authorization error");
            notification.body = errorDescription;
            notification.publish();
            pageStack.pop();
        }
    }

    Notification {
        id: notification
    }
}
