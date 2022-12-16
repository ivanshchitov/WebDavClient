// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ListItem {
    id: root

    property int id
    property alias service: serviceLabel.text
    property alias portalAddress: portalAddressLabel.text
    property string userName
    property string password
    property string selectedText
    property bool busy

    contentHeight: contentColumn.height + Theme.paddingMedium * 2
    acceptedButtons: root.busy ? Qt.NoButton : Qt.LeftButton

    BusyIndicator {
        anchors {
            right: parent.right
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }

        size: BusyIndicatorSize.Small
        running: root.busy
    }

    Column {
        id: contentColumn

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin: Theme.paddingMedium
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
        }

        Item {
            width: parent.width
            height: userNameLabel.height

            Row {
                id: serviceLoginRow

                anchors {
                    left: parent.left
                    right: parent.right
                    rightMargin: Theme.paddingSmall
                }

                width: Math.min(implicitWidth, parent.width - statusRow.width)

                Label {
                    id: userNameLabel

                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeMedium
                    color: root.highlighted ? Theme.highlightColor : Theme.primaryColor
                    text: root.selectedText.length > 0 ? Theme.highlightText(root.name, root.selectedText, Theme.highlightColor) : root.userName
                    textFormat: root.selectedText.length > 0 ? Text.StyledText : Text.PlainText
                }

                Label {
                    id: pointLabel

                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    visible: serviceLabel.text.length > 0 && portalAddressLabel.text.length > 0
                    text: " • "
                }

                Label {
                    id: serviceLabel

                    width: parent.width - userNameLabel.width - pointLabel.width
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeMedium
                    color: root.highlighted ? Theme.highlightColor : Theme.primaryColor
                    visible: text.length > 0
                    textFormat: root.selectedText.length > 0 ? Text.StyledText : Text.PlainText
                }
            }

            Row {
                id: statusRow

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: serviceLoginRow.right
                }

                height: parent.height

                Rectangle {
                    height: 1
                    width: Theme.paddingMedium
                    color: "transparent"
                }
            }
        }

        Label {
            id: portalAddressLabel

            width: parent.width
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            visible: text.length > 0
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.highlightDimmerColor
        opacity: Theme.opacityFaint
        visible: root.busy
    }
}
