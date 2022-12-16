// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QJsonDocument>
#include <QJsonObject>

#include "types/token.h"

Token::Token(QString accessToken, QString refreshToken, long long expiresIn, QDateTime created)
{
    init(accessToken, refreshToken, expiresIn, created);
};

Token::Token(QByteArray binaryData)
{
    QJsonDocument doc = QJsonDocument::fromBinaryData(QByteArray::fromBase64(binaryData));
    if (doc.isObject()) {
        QVariantMap data = doc.toVariant().toMap();
        init(data["access_token"].toString(), data["refresh_token"].toString(),
             data["expires_in"].toLongLong(),
             QDateTime::fromMSecsSinceEpoch(data["created"].toLongLong()));
    }
}

/*!
 *
 * \brief Converts token to a format suitable for storing in persistent memory.
 * \return Hexadeciman representaion of base64 encoded string.
 */
QByteArray Token::serialize() const
{
    QJsonObject data{ { "access_token", m_accessToken },
                      { "refresh_token", m_refreshToken },
                      { "expires_in", m_expiresIn },
                      { "created", m_created.toMSecsSinceEpoch() } };
    return QJsonDocument(data).toBinaryData().toBase64();
}

void Token::init(QString accessToken, QString refreshToken, long long expiresIn, QDateTime created)
{
    m_accessToken = accessToken;
    m_refreshToken = refreshToken;
    m_expiresIn = expiresIn;
    m_created = created;
}
