// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "renamecontroller.h"

/*!
 * \brief RenameController::RenameController Constructor
 * \param parent
 */
RenameController::RenameController(QObject *parent) : ConnectionController(parent) { }

/*!
 * \return Regular expression for input validation
 */
QRegExp RenameController::checkNewName()
{
    QRegExp rx("(?!\\.)[^|\\/<>:\?\\*]{1,246}");
    return rx;
}
