// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "uploadfolderoperation.h"

/*!
 * \brief UploadFolderOperation::UploadFolderOperation Constructor.
 * \param settings Settings with current user data.
 * \param path Path to folder on server.
 * \param folderPath Path to folder on device.
 */
UploadFolderOperation::UploadFolderOperation(Settings *settings, QString &path, QString &folderPath)
    : m_rootFolder(folderPath), m_settings(settings), m_path(path)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs uploading folder operation.
 */
void UploadFolderOperation::run()
{
    if (!m_rootFolder.exists()) {
        emit operationFinished(false,
                               QObject::tr("Folder %1 does not exist").arg(m_rootFolder.dirName()));
        return;
    }

    QNetworkReply *reply = m_webdav.mkdir(m_path + m_rootFolder.dirName());

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        emit operationFinished(false, reply->errorString());
        reply->deleteLater();
        return;
    }

    createFolders();
}

/*!
 * \brief UploadFolderOperation::createFolders Creates all folders except the root one.
 */
void UploadFolderOperation::createFolders()
{
    m_rootFolder.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    QDirIterator iterator(m_rootFolder, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        QNetworkReply *reply = m_webdav.mkdir(m_path + m_rootFolder.dirName()
                                              + iterator.filePath().remove(m_rootFolder.path()));

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NetworkError::NoError) {
            emit operationFinished(false, reply->errorString());
            reply->deleteLater();
            return;
        }
        reply->deleteLater();
    }

    emit creatingFoldersFinished();
    uploadFiles();
}

/*!
 * \brief UploadFolderOperation::uploadFiles Upload all files in folders.
 */
void UploadFolderOperation::uploadFiles()
{
    m_rootFolder.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QDirIterator iterator(m_rootFolder, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        auto document = new QFile(iterator.filePath());
        if (!document->open(QIODevice::ReadOnly)) {
            emit operationFinished(false,
                                   QObject::tr("File %1 does not exist").arg(document->fileName()));
            document->deleteLater();
            return;
        }
        QNetworkReply *reply = m_webdav.put(
                m_path + m_rootFolder.dirName() + iterator.filePath().remove(m_rootFolder.path()),
                document);

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        document->close();
        document->deleteLater();
        if (reply->error() != QNetworkReply::NetworkError::NoError) {
            emit operationFinished(false, reply->errorString());
            reply->deleteLater();
            return;
        }
        reply->deleteLater();
    }
    emit operationFinished(true);
}
