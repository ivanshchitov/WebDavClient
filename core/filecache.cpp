// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QRegExp>

#include "filecache.h"

FileCache::FileCache(const QString &path) : m_cacheDir(path)
{
    if (!m_cacheDir.exists())
        m_cacheDir.mkpath(m_cacheDir.absolutePath());
}

/*!
 * \brief Delete cached file.
 * \param file Relative file path.
 */
void FileCache::deleteFromCache(const QString &file)
{
    m_cacheDir.remove(file);
}

/*!
 * \brief Build file path relatively to the cache root.
 * \param file Relative file path.
 * \return Absolute file path.
 */
QString FileCache::localPath(const QString &filePath)
{
    return m_cacheDir.absoluteFilePath(QFileInfo(filePath).fileName().replace(QRegExp("^/*"), ""));
}
