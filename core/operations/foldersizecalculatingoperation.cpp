// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "foldersizecalculatingoperation.h"

/*!
 * \brief FolderSizeCalculatingOperation::FolderSizeCalculatingOperation
 * \param settings Settings with current user data.
 * \param folderPath Path to folder on server.
 * \param folderName Name of folder on server.
 */
FolderSizeCalculatingOperation::FolderSizeCalculatingOperation(Settings *settings,
                                                               QString &folderPath,
                                                               QString &folderName)
    : m_settings(settings), m_folderPath(folderPath), m_folderName(folderName)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
}

/*!
 * \brief Start folder size calculating.
 */
void FolderSizeCalculatingOperation::run()
{
    m_folderSize = 0;
    calculateFolderSize();
}

/*!
 * \brief FolderSizeCalculatingOperation::calculateFolderSize Calculates folder size.
 * \param folderPath Path to current folder.
 */
void FolderSizeCalculatingOperation::calculateFolderSize(QString folderPath)
{
    if (folderPath.isEmpty())
        folderPath = m_folderPath;

    if (!m_parser.listDirectory(&m_webdav, folderPath)) {
        emit operationFinished(
                false,
                QObject::tr("An unknown error occurred during %1 folder size calculation")
                        .arg(m_folderName));
        return;
    }

    QEventLoop loop;
    connect(&m_parser, &QWebdavDirParser::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QList<QWebdavItem> list = m_parser.getList();
    for (const QWebdavItem &item : list) {
        if (item.isDir())
            calculateFolderSize(item.path());
        else
            m_folderSize += item.size();
    }
    if (folderPath == m_folderPath)
        emit operationFinished(true, "", m_folderSize);
}
