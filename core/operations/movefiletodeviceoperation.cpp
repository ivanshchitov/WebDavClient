// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "movefiletodeviceoperation.h"

/*!
 * \brief MoveFileToDeviceOperation::MoveFileToDeviceOperation Constructor
 * \param settings Settings with current user data.
 * \param path Path to file on server.
 * \param devicePath Path to file on device.
 */
MoveFileToDeviceOperation::MoveFileToDeviceOperation(Settings *settings, QString path,
                                                     QString devicePath)
    : m_settings(settings), m_path(path), m_file(devicePath)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs moving file to device operation with file downloading.
 */
void MoveFileToDeviceOperation::run()
{
    QNetworkReply *reply = m_webdav.get(m_path);
    connect(this, &MoveFileToDeviceOperation::abort, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this,
            [=](qint64 bytesReceived, qint64 bytesTotal) {
                emit progressChanged(double(bytesReceived) / bytesTotal);
            });

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NetworkError::NoError) {
        m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        m_file.write(reply->readAll());
        m_file.close();
        reply->deleteLater();
        emit downloadFinished();
        deleteFile();
    } else {
        emit operationFinished(false, reply->errorString());
        reply->deleteLater();
        return;
    }
}

/*!
 * \brief MoveFileToDeviceOperation::deleteFile Deleting file on server.
 */
void MoveFileToDeviceOperation::deleteFile()
{
    QNetworkReply *reply = m_webdav.remove(m_path);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true, m_path);
        else
            emit operationFinished(false, reply->errorString());
        reply->deleteLater();
    });
}
