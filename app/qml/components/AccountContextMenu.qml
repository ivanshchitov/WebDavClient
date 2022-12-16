// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ContextMenu {
    MenuItem {
        onClicked: {
            remorseDelete(function () {
                    controller.deleteAccount(model.id);
                });
        }

        text: qsTr("Delete")
    }
}
