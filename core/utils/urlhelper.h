// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef URLHELPER_H
#define URLHELPER_H

#include <QObject>
#include <QUrlQuery>
#include <QVariant>

class QQmlEngine;
class QJSEngine;

class UrlHelper : public QObject
{
    Q_OBJECT

public:
    // static methods
    static void registerSingleton(const char *uri);
    static QObject *singletonProvider(QQmlEngine *engine, QJSEngine *);
    Q_INVOKABLE static QVariantMap parseQueryParameters(const QString &query);
    Q_INVOKABLE static QVariantMap parseUrlParameters(const QUrl &url);

private:
    explicit UrlHelper(QObject *parent = nullptr);
    static UrlHelper *s_instance;
};

#endif // URLHELPER_H
