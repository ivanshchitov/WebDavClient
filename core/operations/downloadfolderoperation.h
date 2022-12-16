// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef DOWNLOADFOLDEROPERATION_H
#define DOWNLOADFOLDEROPERATION_H

#include <QDir>
#include <QObject>
#include <QRunnable>
#include <qwebdav.h>
#include <settings.h>

#include "qwebdavdirparser.h"

/*!
 * \brief The DownloadFolderOperation class is operation for folder downloading on device.
 */
class DownloadFolderOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    DownloadFolderOperation(Settings *settings, QString &folderPath, QString &rootPath,
                            bool overwrite);

    void run() override;

private:
    bool overwriteFolder();
    void createFolders(QString folderPath = "");
    void downloadFiles(QString folderPath = "");

private:
    QDir m_rootFolder;
    QWebdav m_webdav;
    Settings *m_settings;
    QWebdavDirParser m_parser;
    const QString &m_folderPath;
    bool m_overwrite;

signals:
    void operationFinished(const bool success, const QString errorString = nullptr);
    void creatingFoldersFinished();
};

#endif // DOWNLOADFOLDEROPERATION_H
