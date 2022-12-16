// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GETUSERINFOOPERATION_H
#define GETUSERINFOOPERATION_H

#include <QObject>
#include <QVariant>
#include <qwebdav.h>
#include "settings.h"

/*!
 * \brief User login retrieval operation.
 */
class GetUserInfoOperation : public QObject
{
    Q_OBJECT

public:
    explicit GetUserInfoOperation(Settings *settings);
    void run();

signals:
    void operationFinished(const QVariantMap userinfo, const QNetworkReply::NetworkError error = {},
                           const QString errorString = {});

private:
    QWebdav m_webdav;
    Settings *m_settings;
    const QString userinfoQuery = "userinfo";
};

#endif // GETUSERINFOOPERATION_H
