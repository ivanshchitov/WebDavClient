// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef UPLOADFILEOPERATION_H
#define UPLOADFILEOPERATION_H

#include <QRunnable>
#include <QObject>

#include "settings.h"
#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"

/*!
 * \brief The UploadFileOperation class is operation for file uploading on server.
 */
class UploadFileOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    UploadFileOperation(Settings *settings, QString &path, QString &documentPath,
                        QString &documentName);

    void run() override;

private:
    QWebdav m_webdav;
    Settings *m_settings;
    const QString &m_path;
    const QString &m_documentPath;
    const QString &m_documentName;

signals:
    void operationFinished(const bool success, const QString errorString = nullptr);
};

#endif // UPLOADFILEOPERATION_H
