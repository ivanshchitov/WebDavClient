// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FILEINFOOPERATION_H
#define FILEINFOOPERATION_H

#include <QObject>

#include "qwebdavdirparser.h"
#include "types/fileorfolderelement.h"
#include "connectioncheckoperation.h"

/*!
 * \brief Remote file info request.
 */
class FileInfoOperation : public QObject
{
    Q_OBJECT
public:
    explicit FileInfoOperation(Settings *settings, const QString &pathe);
    void run();

signals:
    void operationFinished(bool status, QString error, const FileOrFolderElement fileinfo = {});

private slots:
    void handleFileInfoRequest();
    void handleError(QString error);

private:
    QWebdav m_webdav;
    Settings *m_settings;
    QString m_path;
    QWebdavDirParser m_parser;
};

#endif // FILEINFOOPERATION_H
