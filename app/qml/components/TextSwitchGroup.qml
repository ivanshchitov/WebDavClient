// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.0
import Sailfish.Silica 1.0

Repeater {
    property int activeElement

    function _setCheckOfListItems() {
        for (var i = 0; i < model.count; i++) {
            model.get(i).check = model.get(i).value === activeElement;
        }
    }

    TextSwitch {
        id: textSwitch

        onClicked: {
            if (!checked) {
                activeElement = value;
                _setCheckOfListItems();
            }
        }

        text: textItem
        checked: check
        automaticCheck: false
    }

    Component.onCompleted: _setCheckOfListItems()
}
