// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0
import "../components"

Dialog {
    property alias currentPolitic: politics.activeElement

    DialogHeader {
        id: dialogHeader

        objectName: "dialogHeader"
        title: qsTr("Name matching")
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
                width: parent.width
                height: implicitHeight
                text: qsTr("In the current folder there is file with similar name. Select the option that you want.")
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryHighlightColor
                readOnly: true
            }

            TextSwitchGroup {
                id: politics

                width: parent.width
                model: politicsModel
            }
        }

        VerticalScrollDecorator {
        }
    }

    ListModel {
        id: politicsModel

        ListElement {
            value: PoliticsFlags.Overwrite
            textItem: qsTr("Overwrite")
            check: true
        }

        ListElement {
            value: PoliticsFlags.Rename
            textItem: qsTr("Rename")
            check: false
        }
    }
}
