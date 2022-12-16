// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FOLDERSIZECALCULATINGOPERATION_H
#define FOLDERSIZECALCULATINGOPERATION_H

#include <QRunnable>
#include <QObject>

#include "settings.h"
#include "qwebdavdirparser.h"

/*!
 * \brief The FolderSizeCalculatingOperation class is operation for folder size counting.
 */
class FolderSizeCalculatingOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    FolderSizeCalculatingOperation(Settings *settings, QString &folderPath, QString &folderName);

    void run() override;

private:
    void calculateFolderSize(QString folderPath = "");

private:
    quint64 m_folderSize;
    QWebdav m_webdav;
    Settings *m_settings;
    QWebdavDirParser m_parser;
    const QString &m_folderPath;
    const QString &m_folderName;

signals:
    void operationFinished(const bool success, const QString errorString = nullptr,
                           quint64 folderSize = 0L);
};

#endif // FOLDERSIZECALCULATINGOPERATION_H
