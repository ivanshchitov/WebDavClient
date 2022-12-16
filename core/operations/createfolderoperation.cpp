// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "createfolderoperation.h"

/*!
 * \brief CreateFolderOperation::CreateFolderOperation Constructor.
 * \param settings Settings with current user data.
 * \param path Path to new folder.
 */
CreateFolderOperation::CreateFolderOperation(Settings *settings, QString path)
    : m_settings(settings), m_path(path)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs creating folder operation.
 */
void CreateFolderOperation::run()
{
    QNetworkReply *reply = m_webdav.mkdir(m_path);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true);
        else
            emit operationFinished(false, reply->errorString());
        reply->deleteLater();
    });
}
