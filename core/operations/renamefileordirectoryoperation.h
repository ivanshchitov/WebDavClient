// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef RENAMEFILEORDIRECTORYOPERATION_H
#define RENAMEFILEORDIRECTORYOPERATION_H

#include <QRunnable>
#include <QObject>

#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"

/*!
 * \brief The RenameFileOrDirectoryOperation class renames a folder or file
 */
class RenameFileOrDirectoryOperation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RenameFileOrDirectoryOperation(Settings *settings, QString folderPath, QString name,
                                            QString newName);

    void run() override;

private:
    QWebdav m_webdav;
    Settings *m_settings;
    QString m_folderPath;
    QString m_name;
    QString m_newName;

signals:
    void operationFinished(const bool success, QString name, const QString errorString = nullptr);
};

#endif // RENAMEFILEORDIRECTORYOPERATION_H
