// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0
import "../components"

AuthorizationDialog {
    onAccepted: {
        if (acceptDestination) {
            acceptDestinationInstance.service = ServicesFlags.NextCloud;
            acceptDestinationInstance.serverAddress = serverAddress;
            acceptDestinationInstance.username = username;
            acceptDestinationInstance.password = password;
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            addressLabel = qsTr("Enter your NextCloud server address");
            addressPlaceholder = qsTr("NextCloud server address");
        }
    }
}
