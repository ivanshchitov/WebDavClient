// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef MOVEFILETODEVICEOPERATION_H
#define MOVEFILETODEVICEOPERATION_H

#include <QRunnable>
#include <QObject>

#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"

/*!
 * \brief The MoveFileToDeviceOperation class moves a file from server to device
 */
class MoveFileToDeviceOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    MoveFileToDeviceOperation(Settings *settings, QString path, QString devicePath);

    void run() override;

private:
    void deleteFile();

private:
    QWebdav m_webdav;
    Settings *m_settings;
    QString m_path;
    QFile m_file;

signals:
    void downloadFinished();
    void operationFinished(const bool success, const QString errorString = nullptr);
    void progressChanged(float percent);
    void abort();
};

#endif // MOVEFILETODEVICEOPERATION_H
