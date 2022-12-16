// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOADFILESANDFOLDERSOPERATION_H
#define LOADFILESANDFOLDERSOPERATION_H

#include <QRunnable>
#include <QObject>

#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"
#include "qwebdavdirparser.h"

/*!
 * \brief The LoadFilesAndFoldersOperation class is operation for loading
 * files and folders
 */
class LoadFilesAndFoldersOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    LoadFilesAndFoldersOperation(Settings *settings, QString path);

    void run() override;

private slots:
    void handleFilesAndFoldersOperation();

private:
    QWebdav m_webdav;
    Settings *m_settings;
    bool m_success;
    QString m_path;
    QWebdavDirParser m_parser;

signals:
    void operationFinished(bool success, QList<FileOrFolderElement> list = {});
};

#endif // LOADFILESANDFOLDERSOPERATION_H
