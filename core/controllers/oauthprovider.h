// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef ESIAPROVIDER_H
#define ESIAPROVIDER_H

#include <QJSValue>
#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>

#include "types/token.h"

/*!
 * \brief EsiaProvider class manages access token received by signing in using Gosuslugi account.
 */
class OAuthProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl portalUrl READ portalUrl WRITE setPortalUrl NOTIFY portalUrlChanged)
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(
            QString clientSecret READ clientSecret WRITE setClientSecret NOTIFY clientSecretChanged)
    Q_PROPERTY(Token token READ token NOTIFY tokenChanged)
public:
    explicit OAuthProvider(QObject *parent = nullptr);
    Q_INVOKABLE void getAccessToken(QString authorizatonCode);
    Q_INVOKABLE void refreshAccessToken(QString refreshToken);
    QUrl portalUrl() const { return m_portalUrl; }
    QString clientId() const { return m_clientId; }
    QString clientSecret() const { return m_clientSecret; }
    Token token() const { return m_token; }
    void setPortalUrl(QUrl portalUrl);
    void setClientId(QString clientId);
    void setClientSecret(QString clientSecret);

signals:
    void portalUrlChanged(QUrl portalUrl);
    void clientIdChanged(QString clientId);
    void clientSecretChanged(QString clientSecret);
    void tokenChanged(Token token);
    /*!
     * \brief Emitted when service replied with error.
     * \param error Error code.
     * \param errorDescription Error desctiption.
     */
    void errorOccured(QString error, QString errorDescription);

private:
    QUrl m_portalUrl;
    QString m_clientId;
    QString m_clientSecret;
    Token m_token;
    QNetworkAccessManager nam;

    QVariantMap parseReply(QNetworkReply *reply);
    QNetworkReply *postRequest(QUrlQuery query);
    void setToken(QVariantMap params);
    QUrlQuery createAccessTokenParams(QString grantType, QString code);
    void requestToken(QString grantType, QString code);
};

#endif // ESIAPROVIDER_H
