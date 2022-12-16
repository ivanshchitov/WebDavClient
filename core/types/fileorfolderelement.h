// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FILEORFOLDERELEMENT_H
#define FILEORFOLDERELEMENT_H

#include <QString>
#include <QDateTime>
#include <QMetaType>

/*!
 * \brief The FileOrFolderElement universal structure for a file or directory
 */
struct FileOrFolderElement
{
    bool operator==(const FileOrFolderElement &other);
    bool operator!=(const FileOrFolderElement &other);

    QString name;
    QDateTime timestamp;
    QString path;
    bool is_file{ false };
    bool busy{ false };
    quint64 size{ 0 };
    bool selected{ false };
};

Q_DECLARE_METATYPE(FileOrFolderElement);

#endif // FILEORFOLDERELEMENT_H
