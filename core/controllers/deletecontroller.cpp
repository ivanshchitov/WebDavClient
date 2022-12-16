// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "deletecontroller.h"

/*!
 * \brief Constructor
 * \param settings Settings with current user data.
 * \param path Path to the item to be deleted
 * \param isFile true if the item being deleted is a file
 */
DeleteController::DeleteController(Settings *settings, const QString &path, const bool isFile)
    : m_settings(settings), m_path(path), m_isFile(isFile)
{
}

/*!
 * \brief DeleteController::run creates and starts the deletion operation depending on the passed
 * parameters
 */
void DeleteController::run()
{
    if (m_settings->rootPath() == getPathCommunigate() && !m_isFile) {
        auto *operation = new DeleteCommunigateDirectoryOperation(m_settings, m_path);
        connect(operation, &DeleteCommunigateDirectoryOperation::operationFinished, this,
                [=](const bool success, QString path, const QString errorString) {
                    emit finished(success, path, errorString);
                });
        operation->run();
    } else {
        auto *operation = new DeleteFileOrDirectoryOperation(m_settings, m_path);
        connect(operation, &DeleteFileOrDirectoryOperation::operationFinished, this,
                [=](const bool success, QString path, const QString errorString) {
                    emit finished(success, path, errorString);
                });
        operation->run();
    }
}
