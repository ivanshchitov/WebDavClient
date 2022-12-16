// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <qwebdav.h>
#include <qwebdavdirparser.h>

#include "downloadfolderoperation.h"

/*!
 * \brief DownloadFolderOperation::DownloadFolderOperation
 * \param settings Settings with current user data.
 * \param folderPath Path to folder on server.
 * \param rootPath Path to folder on device.
 */
DownloadFolderOperation::DownloadFolderOperation(Settings *settings, QString &folderPath,
                                                 QString &rootPath, bool overwrite)
    : m_rootFolder(rootPath), m_settings(settings), m_folderPath(folderPath), m_overwrite(overwrite)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs downloading folder operation.
 */
void DownloadFolderOperation::run()
{
    if (overwriteFolder()) {
        if (!m_rootFolder.mkpath(".")) {
            emit operationFinished(
                    false, QObject::tr("Error creating folder %1").arg(m_rootFolder.dirName()));
            return;
        }
    } else {
        emit operationFinished(
                false, QObject::tr("Error overwriting folder %1").arg(m_rootFolder.dirName()));
        return;
    }

    createFolders();
}

/*!
 * \brief DownloadFolderOperation::overwriteFolder Deletes the folder to overwrite it.
 * \return true if the operation is successful or overwriting is not required
 */
bool DownloadFolderOperation::overwriteFolder()
{
    if (m_overwrite) {
        if (!m_rootFolder.removeRecursively())
            return false;
        return true;
    }
    return true;
}

/*!
 * \brief DownloadFolderOperation::createFolders Creates all folders.
 * \param folderPath Current folder path.
 */
void DownloadFolderOperation::createFolders(QString folderPath)
{
    if (folderPath.isEmpty())
        folderPath = m_folderPath;

    if (!m_parser.listDirectory(&m_webdav, folderPath)) {
        emit operationFinished(false,
                               QObject::tr("An error occurred during the %1 folder downloading")
                                       .arg(m_rootFolder.dirName()));
        return;
    }

    QEventLoop loop;
    connect(&m_parser, &QWebdavDirParser::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QList<QWebdavItem> list = m_parser.getList();
    for (const QWebdavItem &item : list) {
        if (item.isDir()) {
            if (!m_rootFolder.mkpath(item.path().remove(m_folderPath))) {
                emit operationFinished(false,
                                       QObject::tr("Error creating folder %1").arg(item.name()));
                return;
            }
            createFolders(item.path());
        }
    }
    if (folderPath == m_folderPath) {
        emit creatingFoldersFinished();
        downloadFiles();
    }
}

/*!
 * \brief DownloadFolderOperation::downloadFiles Downloads all files.
 * \param folderPath Current folder path.
 */
void DownloadFolderOperation::downloadFiles(QString folderPath)
{
    if (folderPath.isEmpty())
        folderPath = m_folderPath;

    if (!m_parser.listDirectory(&m_webdav, folderPath)) {
        emit operationFinished(
                false,
                QObject::tr("Unknown error downloading folder %1").arg(m_rootFolder.dirName()));
        return;
    }

    QEventLoop loop;
    connect(&m_parser, &QWebdavDirParser::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QList<QWebdavItem> list = m_parser.getList();
    for (const QWebdavItem &item : list) {
        if (item.isDir())
            downloadFiles(item.path());
        else {
            QNetworkReply *reply = m_webdav.get(item.path());

            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if (reply->error() != QNetworkReply::NetworkError::NoError) {
                emit operationFinished(false, reply->errorString());
                reply->deleteLater();
                return;
            }
            QFile *file = new QFile(m_rootFolder.path() + "/" + item.path().remove(m_folderPath));
            file->open(QIODevice::WriteOnly);
            while (!reply->atEnd())
                file->write(reply->readLine());
            file->close();
            file->deleteLater();
            reply->deleteLater();
        }
    }

    if (folderPath == m_folderPath)
        emit operationFinished(true);
}
