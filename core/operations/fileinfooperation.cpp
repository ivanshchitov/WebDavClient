// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "operations/fileinfooperation.h"
#include "qwebdav.h"

FileInfoOperation::FileInfoOperation(Settings *settings, const QString &path)
    : m_settings(settings), m_path(path)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
    connect(&m_parser, &QWebdavDirParser::finished, this,
            &FileInfoOperation::handleFileInfoRequest);
    connect(&m_parser, &QWebdavDirParser::errorChanged, this, &FileInfoOperation::handleError);
}

/*!
 * \brief Send file info request.
 */
void FileInfoOperation::run()
{
    if (!m_parser.getFileInfo(&m_webdav, m_path))
        emit operationFinished(false, {});
}

/*!
 * \brief Extract file information from file info request.
 */
void FileInfoOperation::handleFileInfoRequest()
{
    const QList<QWebdavItem> files = m_parser.getList();
    for (const QWebdavItem &file : files) {
        FileOrFolderElement fileInfo{
            file.name(), file.lastModified(), file.path(), !file.isDir(), false, file.size(), false,
        };
        emit operationFinished(true, {}, fileInfo);
        return;
    }
    emit operationFinished(false, {});
}

/*!
 * \brief Handle request error.
 * \param error Error description.
 */
void FileInfoOperation::handleError(QString error)
{
    emit operationFinished(false, error);
}
