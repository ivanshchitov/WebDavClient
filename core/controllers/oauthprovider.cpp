// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

#include "controllers/oauthprovider.h"

#define ENDPOINT QStringLiteral("/token")

OAuthProvider::OAuthProvider(QObject *parent) : QObject(parent) { }

/*!
 * \brief Sends HTTP POST request to obtain access token using authorization code.
 * \param authorizatonCode Code received during authorization
 */
void OAuthProvider::getAccessToken(QString authorizatonCode)
{
    requestToken("authorization_code", authorizatonCode);
}

/*!
 * \brief Sends HTTP POST request to refresh access token.
 */
void OAuthProvider::refreshAccessToken(QString refreshToken)
{
    requestToken("refresh_token", refreshToken);
}

void OAuthProvider::setPortalUrl(QUrl portalUrl)
{
    if (m_portalUrl != portalUrl) {
        m_portalUrl = portalUrl;
        emit portalUrlChanged(portalUrl);
    }
}

void OAuthProvider::setClientId(QString clientId)
{
    if (m_clientId != clientId) {
        m_clientId = clientId;
        emit clientIdChanged(clientId);
    }
}

void OAuthProvider::setClientSecret(QString clientSecret)
{
    if (m_clientSecret != clientSecret) {
        m_clientSecret = clientSecret;
        emit clientSecretChanged(clientSecret);
    }
}

/*!
 * \brief Parses JSON structure in the HTTP reply.
 * \param reply HTTP reply.
 * \return Reply parameters map.
 */
QVariantMap OAuthProvider::parseReply(QNetworkReply *reply)
{
    quint64 size = reply->bytesAvailable();
    if (size > 0) {
        QJsonDocument data = QJsonDocument::fromJson(reply->readAll());
        return data.toVariant().toMap();
    }
    return QVariantMap();
}

/*!
 * \brief POSTs request with the given paratemeters to a URL at \l{portalUrl()}.
 * \param data Request parameters.
 * \return Pending request.
 */
QNetworkReply *OAuthProvider::postRequest(QUrlQuery query)
{
    QUrl url(m_portalUrl);
    url.setPath(ENDPOINT);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return nam.post(req, query.toString(QUrl::FullyEncoded).toUtf8());
}

/*!
 * \brief Saves received token.
 * \param params Token map.
 */
void OAuthProvider::setToken(QVariantMap params)
{
    m_token = Token(params["access_token"].toString(), params["refresh_token"].toString(),
                    params["expires_in"].toInt(), QDateTime::currentDateTime());
    emit tokenChanged(m_token);
}

/*!
 * \brief Creates a query with for token request url.
 * \param grantType Request type.
 * \param extra Extra parameters.
 * \return Request query.
 */
QUrlQuery OAuthProvider::createAccessTokenParams(QString grantType, QString code)
{
    QUrlQuery query;
    query.addQueryItem("client_id", m_clientId);
    query.addQueryItem("client_secret", m_clientSecret);
    query.addQueryItem("grant_type", grantType);
    if (grantType == "authorization_code") {
        query.addQueryItem("code", code);
    } else if (grantType == "refresh_token") {
        query.addQueryItem("refresh_token", code);
    }
    return query;
}

/*!
 * \brief Sends token request.
 * \param grantType Request type.
 */
void OAuthProvider::requestToken(QString grantType, QString code)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QUrlQuery requestParams = createAccessTokenParams(grantType, code);
    QNetworkReply *reply = postRequest(requestParams);
    connect(reply, &QNetworkReply::finished, this, [=]() -> void {
        QVariantMap replyParams = parseReply(reply);
        if (reply->error()) {
            if (replyParams.isEmpty())
                emit errorOccured(QString::number(reply->error()), "");
            else
                emit errorOccured(replyParams["error"].toString(),
                                  replyParams["error_description"].toString());
        } else {
            if (replyParams.contains("error"))
                emit errorOccured(replyParams["error"].toString(),
                                  replyParams["error_description"].toString());
            else
                setToken(replyParams);
        }
        reply->deleteLater();
    });
}
