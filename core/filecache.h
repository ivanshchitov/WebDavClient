// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FILECACHE_H
#define FILECACHE_H

#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QGuiApplication>

/*!
 * \brief Local file cache.
 */
class FileCache
{
public:
    explicit FileCache(const QString &path =
                               QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                               + "/" + QGuiApplication::applicationName());

    void deleteFromCache(const QString &file);

    QString localPath(const QString &filePath);

private:
    QDir m_cacheDir;
};

#endif // FILECACHE_H
