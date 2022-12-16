// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "movefoldertodeviceoperation.h"

/*!
 * \brief MoveFolderToDeviceOperation::MoveFolderToDeviceOperation Constructor
 * \param settings Settings with current user data
 * \param path Path to folder on server.
 * \param devicePath Path to folder on device.
 */
MoveFolderToDeviceOperation::MoveFolderToDeviceOperation(Settings *settings, QString &path,
                                                         QString &devicePath, bool overwrite)
    : m_settings(settings), m_folderPath(path), m_rootFolder(devicePath), m_overwrite(overwrite)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs moving folder to device operation.
 */
void MoveFolderToDeviceOperation::run()
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
 * \brief MoveFolderToDeviceOperation::overwriteFolder Deletes the folder to overwrite it
 * \return true if the operation is successful or overwriting is not required
 */
bool MoveFolderToDeviceOperation::overwriteFolder()
{
    if (m_overwrite) {
        if (!m_rootFolder.removeRecursively())
            return false;
        return true;
    }
    return true;
}

/*!
 * \brief MoveFolderToDeviceOperation::createFolders Creates all folders.
 * \param folderPath Current folder path.
 */
void MoveFolderToDeviceOperation::createFolders(QString folderPath)
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
    if (folderPath == m_folderPath)
        downloadFiles();
}

/*!
 * \brief MoveFolderToDeviceOperation::downloadFiles Downloads all files.
 * \param folderPath Current folder path.
 */
void MoveFolderToDeviceOperation::downloadFiles(QString folderPath)
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
        deleteFolder();
}

/*!
 * \brief MoveFolderToDeviceOperation::deleteFolder Deleting folder on server.
 */
void MoveFolderToDeviceOperation::deleteFolder()
{
    QNetworkReply *reply = m_webdav.remove(m_folderPath);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NetworkError::NoError)
            emit operationFinished(true, m_folderPath);
        else
            emit operationFinished(false, reply->errorString());
        reply->deleteLater();
    });
}
