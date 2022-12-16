// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "connectioncheckoperation.h"

/*!
 * \brief Contructor
 * \param settings Settings with current user data.
 */
ConnectionCheckOperation::ConnectionCheckOperation(Settings *settings) : m_settings(settings)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
}

/*!
 * \brief ConnectionCheckOperation::checkConnection Tests connection to the specified
 * WedDAV server
 */
void ConnectionCheckOperation::checkConnection()
{
    QNetworkReply *reply = m_webdav.list("/");
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true);
        else
            emit operationFinished(false, reply->errorString());
        reply->deleteLater();
    });
}
