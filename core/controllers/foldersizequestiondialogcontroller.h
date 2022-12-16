// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FOLDERSIZEQUESTIONDIALOGCONTROLLER_H
#define FOLDERSIZEQUESTIONDIALOGCONTROLLER_H

#include "connectioncontroller.h"

/*!
 * \brief The FolderSizeQuestionDialogController class is a controller for FolderSizeQuestionDialog.
 */
class FolderSizeQuestionDialogController : public ConnectionController
{
    Q_OBJECT
    Q_PROPERTY(QString errorMessage READ errorMessage);
    Q_PROPERTY(QString msgTitle READ msgTitle);
    Q_PROPERTY(quint64 folderSize READ folderSize);
    MAKE_INTERFACE_PROPERTY(Settings *, settings, Settings);
    MAKE_INTERFACE_PROPERTY(QString, path, Path);

public:
    FolderSizeQuestionDialogController(QObject *parent = nullptr);

    QString errorMessage();
    QString msgTitle();
    quint64 folderSize();

    Q_INVOKABLE void runFolderSizeCalculatingOperation(QString folderName);

private slots:
    void handleFolderSizeCalculatingOperation(const bool success,
                                              const QString errorString = nullptr,
                                              const quint64 folderSize = 0L);

private:
    quint64 m_folderSize;
    QString m_errorMessage;
    QString m_msgTitle;

signals:
    void entitiesModelChanged();
    void operationError();
    void operationSuccess();
};

#endif // FOLDERSIZEQUESTIONDIALOGCONTROLLER_H
