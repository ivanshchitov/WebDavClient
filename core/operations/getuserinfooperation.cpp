// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "operations/getuserinfooperation.h"
#include "qwebdav.h"

/*!
 * \brief GetUserInfoOperation::GetUserInfoOperation Contructor.
 * \param settings Settings with current user data.
 */
GetUserInfoOperation::GetUserInfoOperation(Settings *settings) : m_settings(settings)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Sends GET request to /?userinfo path
 */
void GetUserInfoOperation::run()
{
    QNetworkReply *reply = m_webdav.get("/", userinfoQuery);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            QByteArray data = reply->readAll();
            QVariantMap userinfo;
            for (QByteArray &item : data.split('\n')) {
                QList<QByteArray> param = item.split(':');
                if (param.length() > 1)
                    userinfo[param[0]] = param[1];
            }
            emit operationFinished(userinfo);
        } else {
            emit operationFinished({}, reply->error(), reply->errorString());
        }
        reply->deleteLater();
    });
}
