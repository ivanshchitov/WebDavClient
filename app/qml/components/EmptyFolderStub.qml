// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: root

    property string title
    property string label

    Loader {
        anchors.fill: parent

        sourceComponent: orientation === (Orientation.Portrait || Orientation.PortraitInverted) ? verticalStub : horizontalStub
        active: true
    }

    Component {
        id: horizontalStub

        Item {
            Row {
                anchors.horizontalCenter: parent.horizontalCenter

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    width: emptyIcon.width

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter

                        text: root.title
                        color: Theme.highlightColor
                        textFormat: Text.PlainText
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }

                    TextArea {
                        anchors.horizontalCenter: parent.horizontalCenter

                        text: root.label
                        readOnly: true
                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }
                }
            }

            Behavior on opacity  {
                PropertyAnimation {
                }
            }
        }
    }

    Component {
        id: verticalStub

        Column {
            Label {
                anchors.horizontalCenter: parent.horizontalCenter

                text: root.title
                color: Theme.highlightColor
                textFormat: Text.PlainText
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            TextArea {
                anchors.horizontalCenter: parent.horizontalCenter

                text: root.label
                readOnly: true
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Behavior on opacity  {
                PropertyAnimation {
                }
            }
        }
    }
}
