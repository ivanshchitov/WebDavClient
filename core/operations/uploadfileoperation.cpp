// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "uploadfileoperation.h"

/*!
 * \brief UploadFileOperation::UploadFileOperation Constructor.
 * \param settings Settings with current user data.
 * \param path Path to document on server.
 * \param documentPath Path to document on device.
 * \param documentName Name of the document.
 */
UploadFileOperation::UploadFileOperation(Settings *settings, QString &path, QString &documentPath,
                                         QString &documentName)
    : m_settings(settings), m_path(path), m_documentPath(documentPath), m_documentName(documentName)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs uploading file operation.
 */
void UploadFileOperation::run()
{
    auto *document = new QFile(m_documentPath);

    if (!document->open(QIODevice::ReadOnly)) {
        emit operationFinished(false,
                               QObject::tr("File %1 does not exist").arg(document->fileName()));
        document->deleteLater();
        return;
    }

    QNetworkReply *reply = m_webdav.put(m_path + m_documentName, document);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true);
        else
            emit operationFinished(false, reply->errorString());
        document->close();
        document->deleteLater();
        reply->deleteLater();
    });
}
