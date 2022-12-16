// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "renamefileordirectoryoperation.h"

/*!
 * \brief Constructor
 * \param settings Settings with current user data.
 * \param folderPath the path to the folder where the item is located
 * \param name original name of the element
 * \param newName new name of the element
 */
RenameFileOrDirectoryOperation::RenameFileOrDirectoryOperation(Settings *settings,
                                                               QString folderPath, QString name,
                                                               QString newName)
    : m_settings(settings), m_folderPath(folderPath), m_name(name), m_newName(newName)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs renaming files or folder operation.
 */
void RenameFileOrDirectoryOperation::run()
{
    QNetworkReply *reply = m_webdav.move(m_folderPath + m_name, m_folderPath + m_newName);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true, m_name);
        else
            emit operationFinished(false, m_name, reply->errorString());
        reply->deleteLater();
    });
}
