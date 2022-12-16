// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "downloadfileoperation.h"
#include <qwebdav.h>

/*!
 * \brief DownloadFileOperation::DownloadFileOperation Constructor.
 * \param settings Settings with current user data.
 * \param path Path to document on server.
 */
DownloadFileOperation::DownloadFileOperation(Settings *settings, const QString &path,
                                             const QString &filePath)
    : m_settings(settings), m_path(path), m_filePath(filePath)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Start downloading file.
 */
void DownloadFileOperation::run()
{
    m_file.setFileName(m_filePath);
    m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QNetworkReply *reply = m_webdav.get(m_path);
    connect(this, &DownloadFileOperation::abort, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this,
            [=](qint64 bytesReceived, qint64 bytesTotal) {
                emit progressChanged(double(bytesReceived) / bytesTotal);
            });
    connect(reply, &QNetworkReply::readyRead, this, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            m_file.write(reply->readAll());
            m_file.flush();
        }
    });
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError) {
            m_file.write(reply->readAll());
            m_file.flush();
            emit operationFinished(true);
        } else {
            emit operationFinished(false, reply->errorString());
        }
        m_file.close();
        reply->deleteLater();
    });
}
