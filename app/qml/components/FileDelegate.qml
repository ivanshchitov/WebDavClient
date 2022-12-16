// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.auroraos.AuroraOsWebDavGuiClient 1.0

ListItem {
    id: root

    property bool isFile: model.is_file === undefined ? true : model.is_file
    property alias timestamp: timeLabel.text
    property alias iconSource: itemIcon.source
    property alias size: sizeLabel.text
    property string selectedText
    property string name
    property string path
    property bool busy

    function getIconByFileExtension(extension) {
        switch (extension) {
        case "rpm":
            return "image://theme/icon-m-file-rpm";
        case "apk":
            return "image://theme/icon-m-file-apk";
        case "pdf":
            return "image://theme/icon-m-file-pdf";
        case "flac":
        case "mp3":
        case "wav":
            return "image://theme/icon-m-file-audio";
        case "3gp":
        case "avi":
        case "mkv":
        case "mp4":
        case "webm":
            return "image://theme/icon-m-file-video";
        case "bmp":
        case "gif":
        case "ico":
        case "jpeg":
        case "jpg":
        case "png":
        case "svg":
        case "tiff":
        case "webp":
        case "eps":
            return "image://theme/icon-m-file-image";
        case "docx":
        case "doc":
        case "rtf":
        case "odt":
            return "image://theme/icon-m-file-formatted";
        case "xlsx":
        case "xls":
        case "ods":
            return "image://theme/icon-m-file-spreadsheet";
        case "pptx":
        case "ppt":
        case "odp":
            return "image://theme/icon-m-file-presentation";
        case "txt":
            return "image://theme/icon-m-file-note";
        case "tar":
        case "zip":
        case "7z":
            return "image://theme/icon-m-file-archive-folder";
        default:
            return "image://theme/icon-m-file-other";
        }
    }

    contentHeight: contentColumn.height + Theme.paddingLarge * 2
    acceptedButtons: root.busy ? Qt.NoButton : Qt.LeftButton

    Rectangle {
        id: iconRectangle

        anchors.left: parent.left

        height: parent.height
        width: root.contentHeight

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Theme.lightSecondaryColor
            }
            GradientStop {
                position: 1.0
                color: Theme.darkSecondaryColor
            }
        }
        opacity: Theme.opacityFaint
    }

    Icon {
        id: itemIcon

        anchors.centerIn: iconRectangle

        source: root.isFile ? getIconByFileExtension(root.name.split(".").pop()) : "image://theme/icon-m-file-folder"
        fillMode: Image.PreserveAspectFit
        visible: !root.busy
    }

    BusyIndicator {
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: iconRectangle.horizontalCenter
        }

        size: BusyIndicatorSize.Small
        running: root.busy
    }

    Column {
        id: contentColumn

        anchors {
            top: parent.top
            left: iconRectangle.right
            right: parent.right
            topMargin: Theme.paddingLarge
            leftMargin: Theme.paddingMedium
            rightMargin: Theme.horizontalPageMargin
        }

        spacing: Theme.paddingSmall

        Item {
            width: parent.width
            height: nameLabel.height

            Label {
                id: nameLabel

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeMedium
                color: root.highlighted ? Theme.highlightColor : Theme.primaryColor
                text: root.selectedText.length > 0 ? Theme.highlightText(root.name, root.selectedText, Theme.highlightColor) : root.name
                textFormat: root.selectedText.length > 0 ? Text.StyledText : Text.PlainText
                width: Math.min(implicitWidth, parent.width - statusRow.width)
            }

            Row {
                id: statusRow

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: nameLabel.right
                }

                height: parent.height

                Rectangle {
                    height: 1
                    width: Theme.paddingMedium
                    color: "transparent"
                }
            }
        }

        Row {
            Label {
                id: timeLabel

                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                visible: text.length > 0
            }

            Label {
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                visible: timeLabel.text.length > 0 && sizeLabel.text.length > 0 && root.isFile
                text: ", "
            }

            Label {
                id: sizeLabel

                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                visible: text.length > 0 && root.isFile
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.highlightDimmerColor
        opacity: Theme.opacityFaint
        visible: root.busy
    }
}
