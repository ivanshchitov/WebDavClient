// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef TOKEN_H
#define TOKEN_H

#include <QDateTime>
#include <QVariant>

/*!
 * \brief Token class holds OAuth token data.
 */
class Token
{
    Q_GADGET
    Q_PROPERTY(QString accessToken READ accessToken CONSTANT)
    Q_PROPERTY(QString refreshToken READ refreshToken CONSTANT)
    Q_PROPERTY(long long expiresIn READ expiresIn CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)

public:
    Token() { }
    Token(QString accessToken, QString refreshToken, long long expiresIn, QDateTime created);
    Token(QByteArray binaryData);
    QByteArray serialize() const;
    QString accessToken() const { return m_accessToken; }
    QString refreshToken() const { return m_refreshToken; }
    long long expiresIn() const { return m_expiresIn; }
    QDateTime created() const { return m_created; }

private:
    QString m_accessToken;
    QString m_refreshToken;
    long long m_expiresIn;
    QDateTime m_created;

    void init(QString accessToken, QString refreshToken, long long expiresIn, QDateTime created);
};

Q_DECLARE_METATYPE(Token)

#endif // TOKEN_H
