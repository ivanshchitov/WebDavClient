// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "movefileordirectoryoperation.h"

/*!
 * \brief Constructor
 * \param settings Settings with current user data.
 * \param path Path to the element to be moved
 * \param newPath New path of the element being moved
 * \param overwrite Permission to overwrite
 */
MoveFileOrDirectoryOperation::MoveFileOrDirectoryOperation(Settings *settings, QString path,
                                                           QString newPath, bool overwrite)
    : m_settings(settings), m_path(path), m_newPath(newPath), m_overwrite(overwrite)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs moving a file or directory
 */
void MoveFileOrDirectoryOperation::run()
{
    QNetworkReply *reply = m_webdav.move(m_path, m_newPath, m_overwrite);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true, m_path);
        else
            emit operationFinished(false, m_path, reply->errorString());
        reply->deleteLater();
    });
}
