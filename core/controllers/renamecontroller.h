// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef RENAMECONTROLLER_H
#define RENAMECONTROLLER_H

#include <QObject>

#include "connectioncontroller.h"

class RenameController : public ConnectionController
{
    Q_OBJECT
public:
    explicit RenameController(QObject *parent = nullptr);

    Q_INVOKABLE QRegExp checkNewName();
};

#endif // RENAMECONTROLLER_H
