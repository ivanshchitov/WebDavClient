// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef UPLOADFOLDEROPERATION_H
#define UPLOADFOLDEROPERATION_H

#include <QObject>
#include <QRunnable>

#include "connectioncheckoperation.h"

/*!
 * \brief The UploadFolderOperation class is operation for folder uploading on
 * server.
 */
class UploadFolderOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    UploadFolderOperation(Settings *settings, QString &path, QString &folderPath);

    void run() override;

private:
    void createFolders();
    void uploadFiles();

private:
    QDir m_rootFolder;
    QWebdav m_webdav;
    Settings *m_settings;
    const QString &m_path;

signals:
    void operationFinished(const bool success, const QString errorString = nullptr);
    void creatingFoldersFinished();
};

#endif // UPLOADFOLDEROPERATION_H
