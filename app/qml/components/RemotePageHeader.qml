// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

PageHeader {
    id: root

    property int delay: 750
    property int duration: 400
    property bool display
    property bool busy

    onBusyChanged: {
        root.display = root.busy;
        if (busyTimer.running) {
            busyTimer.restart();
            return;
        }
        busyTimer.start();
    }

    leftMargin: (busyIndicator.visible ? (busyIndicator.x + busyIndicator.width) : 0) + Theme.horizontalPageMargin

    BusyIndicator {
        id: busyIndicator

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: Theme.paddingLarge * 2
        }
        size: BusyIndicatorSize.Small
        running: visible
        visible: opacity > 0.01
        opacity: root.display ? 1.0 : 0.0

        Behavior on opacity  {
            NumberAnimation {
                duration: root.duration
            }
        }

        Timer {
            id: busyTimer

            onTriggered: busyIndicator.opacity = root.display ? 1.0 : 0.0

            interval: root.delay
            repeat: false
            running: false
        }
    }
}
