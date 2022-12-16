// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "deletefileordirectoryoperation.h"

/*!
 * \brief Constructor
 * \param settings Settings with current user data.
 * \param pathElement Path to the item to be deleted
 */
DeleteFileOrDirectoryOperation::DeleteFileOrDirectoryOperation(Settings *settings,
                                                               QString pathElement)
    : m_settings(settings), m_path(pathElement)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs deleting a file or directory at the specified path
 */
void DeleteFileOrDirectoryOperation::run()
{
    QNetworkReply *reply = m_webdav.remove(m_path);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true, m_path);
        else
            emit operationFinished(false, m_path, reply->errorString());
        reply->deleteLater();
    });
}
