// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef DELETECOMMUNIGATEDIRECTORYOPERATION_H
#define DELETECOMMUNIGATEDIRECTORYOPERATION_H

#include <QRunnable>
#include <QObject>

#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"
#include "qwebdavdirparser.h"

/*!
 * \brief The DeleteCommunigateDirectoryOperation class deletes directories for the Communigate
 * service
 */
class DeleteCommunigateDirectoryOperation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    DeleteCommunigateDirectoryOperation(Settings *settings, QString pathElement);

    void run() override;

private:
    void deleteFileOrFolder(QString filePath);
    void clearFolder(QString folderPath);

private:
    QWebdav m_webdav;
    QWebdavDirParser m_parser;
    Settings *m_settings;
    QString m_rootFolder;

signals:
    void operationFinished(const bool success, QString path, const QString errorString = nullptr);
};

#endif // DELETECOMMUNIGATEDIRECTORYOPERATION_H
