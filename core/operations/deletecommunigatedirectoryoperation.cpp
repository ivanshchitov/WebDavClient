// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "deletecommunigatedirectoryoperation.h"

/*!
 * \brief Constructor
 * \param settings Settings with current user data.
 * \param pathElement Path to the item to be deleted
 */
DeleteCommunigateDirectoryOperation::DeleteCommunigateDirectoryOperation(Settings *settings,
                                                                         QString pathElement)
    : m_settings(settings), m_rootFolder(pathElement)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Runs deleting a directory by the passed path
 */
void DeleteCommunigateDirectoryOperation::run()
{
    clearFolder(m_rootFolder);
}

/*!
 * \brief DeleteCommunigateDirectoryOperation::clearFolder Clears the deleted folder, first
 * deleting all internal files, and then the folder itself
 * \param folderPath The path to the directory to be cleaned
 */
void DeleteCommunigateDirectoryOperation::clearFolder(QString folderPath)
{
    if (!m_parser.listDirectory(&m_webdav, folderPath)) {
        emit operationFinished(false, folderPath);
        return;
    }

    QEventLoop loop;
    connect(&m_parser, &QWebdavDirParser::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QList<QWebdavItem> list = m_parser.getList();
    int numberDeletedItems = 0;
    for (const QWebdavItem &item : list) {
        if (item.isDir()) {
            clearFolder(item.path());
        } else
            deleteFileOrFolder(item.path());
        numberDeletedItems++;
    }

    if (list.size() != numberDeletedItems) {
        emit operationFinished(false, folderPath);
        return;
    }
    deleteFileOrFolder(folderPath);

    if (folderPath == m_rootFolder)
        emit operationFinished(true, folderPath);
}

/*!
 * \brief DeleteCommunigateDirectoryOperation::deleteFileOrFolder Deletes the file or folder by the
 * passed path
 * \param filePath The path to the file or folder to be deleted
 */
void DeleteCommunigateDirectoryOperation::deleteFileOrFolder(QString filePath)
{
    QNetworkReply *reply = m_webdav.remove(filePath);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        emit operationFinished(false, filePath, reply->errorString());
        reply->deleteLater();
        return;
    }
}
