// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SELECTFOLDERDIALOGCONTROLLER_H
#define SELECTFOLDERDIALOGCONTROLLER_H

#include <QObject>

#include "controllers/folderpagecontroller.h"

/*!
 * \brief The SelectFolderDialogController class is a controller for the select folder dialog.
 */
class SelectFolderDialogController : public FolderPageController
{
    Q_OBJECT

public:
    SelectFolderDialogController(QObject *parent = nullptr);

    Q_INVOKABLE bool abilityToReturn(QString path);
    Q_INVOKABLE bool abilityToInteract(FileOrFolderElement element, QString path, bool isFile);
    Q_INVOKABLE QString returnParentPath(QString path);
    Q_INVOKABLE QString returnParentName(QString path);
};

#endif // SELECTFOLDERDIALOGCONTROLLER_H
