// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef DELETEFILEORDIRECTORYOPERATION_H
#define DELETEFILEORDIRECTORYOPERATION_H

#include <QRunnable>
#include <QObject>

#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"

/*!
 * \brief The DeleteFileOrDirectoryOperation class deletes a folder or file
 */
class DeleteFileOrDirectoryOperation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit DeleteFileOrDirectoryOperation(Settings *settings, QString pathElement);

    void run() override;

private:
    QWebdav m_webdav;
    Settings *m_settings;
    QString m_path;

signals:
    void operationFinished(const bool success, QString path, const QString errorString = nullptr);
};

#endif // DELETEFILEORDIRECTORYOPERATION_H
