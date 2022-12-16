// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CONNECTIONCHECKOPERATION_H
#define CONNECTIONCHECKOPERATION_H

#include <QObject>

#include <qwebdav.h>
#include "settings.h"

/*!
 * \brief Connection verification operation
 */
class ConnectionCheckOperation : public QObject
{
    Q_OBJECT

public:
    explicit ConnectionCheckOperation(Settings *settings);
    void checkConnection();

signals:
    void operationFinished(const bool success, const QString errorString = {});

private:
    QWebdav m_webdav;
    Settings *m_settings;
};

#endif // CONNECTIONCHECKOPERATION_H
