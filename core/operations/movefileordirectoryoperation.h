// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef MOVEFILEORDIRECTORYOPERATION_H
#define MOVEFILEORDIRECTORYOPERATION_H

#include <QRunnable>
#include <QObject>

#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"

/*!
 * \brief The MoveFileOrDirectoryOperation class moves a folder or file
 */
class MoveFileOrDirectoryOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    MoveFileOrDirectoryOperation(Settings *settings, QString path, QString newPath, bool overwrite);

    void run() override;

private:
    QWebdav m_webdav;
    Settings *m_settings;
    QString m_path;
    QString m_newPath;
    bool m_overwrite;

signals:
    void operationFinished(const bool success, QString path, const QString errorString = nullptr);
};

#endif // MOVEFILEORDIRECTORYOPERATION_H
