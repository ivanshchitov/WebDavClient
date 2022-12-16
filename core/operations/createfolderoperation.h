// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CREATEFOLDEROPERATION_H
#define CREATEFOLDEROPERATION_H

#include <QRunnable>
#include <QObject>

#include "connectioncheckoperation.h"
#include "types/fileorfolderelement.h"

/*!
 * \brief The CreateFolderOperation class is operation for folder creation.
 */
class CreateFolderOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    CreateFolderOperation(Settings *settings, QString path);

    void run() override;

private:
    QWebdav m_webdav;
    Settings *m_settings;
    QString m_path;

signals:
    void operationFinished(const bool success, const QString errorString = nullptr);
};

#endif // CREATEFOLDEROPERATION_H
