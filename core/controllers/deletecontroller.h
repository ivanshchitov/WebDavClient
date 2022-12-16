// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef DELETECONTROLLER_H
#define DELETECONTROLLER_H

#include "connectioncontroller.h"
#include "../operations/deletecommunigatedirectoryoperation.h"
#include "../operations/deletefileordirectoryoperation.h"

/*!
 * \brief The DeleteController class handles the deletion process
 */
class DeleteController : public ConnectionController
{
    Q_OBJECT
public:
    explicit DeleteController(Settings *settings, const QString &path, const bool isFile);

public slots:
    void run();

signals:
    void finished(const bool success, QString path, const QString errorString = nullptr);

private:
    Settings *m_settings;
    QString m_path;
    bool m_isFile;
};

#endif // DELETECONTROLLER_H
