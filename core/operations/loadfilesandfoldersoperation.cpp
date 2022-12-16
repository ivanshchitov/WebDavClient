// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "loadfilesandfoldersoperation.h"

/*!
 * \brief Constructor
 * \param settings Settings with current user data.
 * \param path Path to current folder
 */
LoadFilesAndFoldersOperation::LoadFilesAndFoldersOperation(Settings *settings, QString path)
    : m_settings(settings), m_path(path)
{
    m_webdav.setConnectionSettings(m_settings->connectionType(), m_settings->hostName(),
                                   m_settings->rootPath(), m_settings->userName(),
                                   m_settings->password(), m_settings->port().toInt());
    m_webdav.setAuthorizationType(m_settings->authType());
    m_webdav.setToken(m_settings->token());
    connect(&m_parser, &QWebdavDirParser::finished, this,
            &LoadFilesAndFoldersOperation::handleFilesAndFoldersOperation);
}

/*!
 * \brief Runs files and folders loading operation for current folder.
 */
void LoadFilesAndFoldersOperation::run()
{
    m_success = m_parser.listDirectory(&m_webdav, m_path);
}

/*!
 * \brief Transforms reply from parser into QList<FileOrFolderElement>
 */
void LoadFilesAndFoldersOperation::handleFilesAndFoldersOperation()
{

    if (!m_success)
        emit operationFinished(false);

    const QList<QWebdavItem> list = m_parser.getList();
    QList<FileOrFolderElement> entities;
    for (const QWebdavItem &item : list) {
        FileOrFolderElement entity;
        entity.name = item.name();
        entity.timestamp = item.lastModified();
        entity.selected = false;
        entity.busy = false;
        entity.path = item.path();

        auto isDir = item.isDir();
        if (isDir)
            entity.is_file = false;
        else {
            entity.is_file = true;
            entity.size = item.size();
        }

        entities.append(entity);
    }

    emit operationFinished(true, entities);
}
