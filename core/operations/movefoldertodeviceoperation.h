// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef MOVEFOLDERTODEVICEOPERATION_H
#define MOVEFOLDERTODEVICEOPERATION_H

#include <QDir>
#include <QObject>
#include <QRunnable>
#include <qwebdav.h>
#include <settings.h>

#include "qwebdavdirparser.h"

/*!
 * \brief The MoveFolderToDeviceOperation class moves a folder from server to device
 */
class MoveFolderToDeviceOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    MoveFolderToDeviceOperation(Settings *settings, QString &path, QString &devicePath,
                                bool overwrite);

    void run() override;

private:
    bool overwriteFolder();
    void createFolders(QString folderPath = "");
    void downloadFiles(QString folderPath = "");
    void deleteFolder();

private:
    QWebdav m_webdav;
    QWebdavDirParser m_parser;
    Settings *m_settings;
    QString m_folderPath;
    QDir m_rootFolder;
    bool m_overwrite;

signals:
    void downloadFinished();
    void operationFinished(const bool success, const QString errorString = nullptr);
};

#endif // MOVEFOLDERTODEVICEOPERATION_H
