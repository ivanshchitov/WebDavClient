// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ApplicationWindow {
    objectName: "applicationWindow"
    initialPage: Qt.resolvedUrl("pages/AccountSelectionPage.qml")
    cover: Qt.resolvedUrl("cover/DefaultCoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Settings {
        id: appSettings
    }

    DataAccessObject {
        id: appDao
    }

    Migrations {
        db: appDao.db
        Component.onCompleted: performMigration()
    }

    /**
     * Returns a page from the page stack by the page object name.
     * For example: "localPage", "mainPage", etc.
     */
    function findPageByObjectName(pageObjectName) {
        return pageStack.find(function (page) {
                return page.objectName.toString().indexOf(pageObjectName) !== -1;
            });
    }
}
