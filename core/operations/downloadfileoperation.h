// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef DOWNLOADFILEOPERATION_H
#define DOWNLOADFILEOPERATION_H

#include <QObject>
#include <QFile>
#include "connectioncheckoperation.h"

class QWebdav;

/*!
 * \brief File downloading operation.
 */
class DownloadFileOperation : public QObject
{
    Q_OBJECT

public:
    explicit DownloadFileOperation(Settings *settings, const QString &path,
                                   const QString &filePath);
    void run();

signals:
    void operationFinished(const bool success, const QString errorString = {});
    void progressChanged(float percent);
    void abort();

private:
    QWebdav m_webdav;
    Settings *m_settings;
    QString m_path;
    QString m_filePath;
    QFile m_file;
};

#endif // DOWNLOADFILEOPERATION_H
