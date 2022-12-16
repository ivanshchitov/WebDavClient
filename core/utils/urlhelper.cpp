// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "utils/urlhelper.h"

#include <QtQml>

UrlHelper *UrlHelper::s_instance;

/*!
 * \brief UrlHelper::UrlHelper constructor
 * \param parent
 */
UrlHelper::UrlHelper(QObject *parent) : QObject(parent) { }

/*!
 * \brief UrlHelper::registerSingleton
 * \param uri
 */
void UrlHelper::registerSingleton(const char *uri)
{
    qmlRegisterSingletonType<UrlHelper>(uri, 1, 0, "UrlHelper", UrlHelper::singletonProvider);
}

/*!
 * \brief UrlHelper::registerSingleton
 * \param uri
 */
QObject *UrlHelper::singletonProvider(QQmlEngine *engine, QJSEngine *)
{
    if (!s_instance)
        s_instance = new UrlHelper(engine);

    return s_instance;
}

/*!
 * \brief UrlHelper::parseQueryParameters Extract URL parameters into a map from a URL query string
 * \param query URL query
 * \return Map with url parameters
 */
QVariantMap UrlHelper::parseQueryParameters(const QString &query)
{
    QUrlQuery urlQuery(query);
    QVariantMap parameters = {};
    for (QPair<QString, QString> &pair : urlQuery.queryItems(QUrl::FullyDecoded))
        parameters[pair.first] = pair.second;
    return parameters;
}

/*!
 * \brief UrlHelper::parseQueryParameters Extract URL parameters into a map from a URL string
 * \param query URL string
 * \return Map with url parameters
 */
QVariantMap UrlHelper::parseUrlParameters(const QUrl &url)
{
    return parseQueryParameters(url.query());
}
