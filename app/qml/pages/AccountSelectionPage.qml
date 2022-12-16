// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0
import "../components"

Page {
    id: root

    onStatusChanged: {
        if (status === PageStatus.Active)
            controller.updateModelData();
    }

    objectName: "AccountSelectionPage"

    AccountSelectionController {
        id: controller

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

        dao: appDao
    }

    SilicaFlickable {
        id: contentContainer

        anchors.fill: parent
        Loader {
            id: pageMenuLoader

            sourceComponent: AccountPagePullMenu {
            }
        }

        RemotePageHeader {
            id: pageHeader

            objectName: "pageHeader"
            title: qsTr("Account selection")
            display: false
            extraContent.children: [
                IconButton {
                    objectName: "aboutButton"
                    icon.source: "image://theme/icon-m-about"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: pageStack.push(Qt.resolvedUrl("../pages/AboutPage.qml"))
                }
            ]
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
            model: controller.remoteServiceModel

            delegate: AccountDelegate {
                onClicked: pageStack.push(Qt.resolvedUrl("AuthorizationLoadingPage.qml"), {
                        "service": returnNumService(model.service),
                        "serverAddress": model.portalAddress,
                        "username": model.userName,
                        "password": model.password,
                        "token": model.token
                    })

                service: model.service
                portalAddress: model.portalAddress
                userName: model.userName
                password: model.password
                busy: model.busy
                menu: menuLoader.item

                Loader {
                    id: menuLoader

                    source: Qt.resolvedUrl("../components/AccountContextMenu.qml")
                    active: true
                }
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

            title: qsTr("No login accounts")
            label: qsTr("Add an account to save it for logging in in the future.")
            visible: documentsView.count <= 0 && !pageHeader.busy
        }
    }

    Notification {
        id: notification
    }

    function returnNumService(service) {
        switch (service) {
        case "NextCloud":
            return ServicesFlags.NextCloud;
        case "YandexDisk":
            return ServicesFlags.YandexDisk;
        case "Communigate":
            return ServicesFlags.Communigate;
        case "WebDavServer":
            return ServicesFlags.WebDavServer;
        }
    }
}
