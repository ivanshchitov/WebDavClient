// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <qwebdav.h>
#include <QStandardPaths>
#include <QSettings>

#define CUSTOM_SETTINGS_PATH                                                                       \
    QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)                            \
            + QStringLiteral("/appConfig.ini")

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString port READ port WRITE savePort NOTIFY portChanged)
    Q_PROPERTY(QString hostName READ hostName WRITE saveHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString rootPath READ rootPath WRITE saveRootPath NOTIFY rootPathChanged)
    Q_PROPERTY(QString userName READ userName WRITE saveUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE savePassword NOTIFY passwordChanged)
    Q_PROPERTY(QString token READ token WRITE saveToken NOTIFY tokenChanged)
    Q_PROPERTY(QWebdav::QWebdavAuthorizationType authType READ authType WRITE saveAuthType NOTIFY
                       authTypeChanged)
    Q_PROPERTY(QWebdav::QWebdavConnectionType connectionType READ connectionType WRITE
                       saveConnectionType NOTIFY connectionTypeChanged)

public:
    explicit Settings(const QString &confFilePath = CUSTOM_SETTINGS_PATH,
                      QObject *parent = nullptr);

    // Setters
    void savePort(const QString port);
    void saveHostName(const QString &hostName);
    void saveRootPath(const QString &rootPath);
    void saveUserName(const QString &userName);
    void savePassword(const QString &password);
    void saveToken(const QString &token);
    void saveAuthType(QWebdav::QWebdavAuthorizationType authType);
    void saveConnectionType(QWebdav::QWebdavConnectionType connectionType);

    // Getters
    QString port() const;
    QString hostName() const;
    QString rootPath() const;
    QString userName() const;
    QString password() const;
    QString token() const;
    QWebdav::QWebdavAuthorizationType authType() const;
    QWebdav::QWebdavConnectionType connectionType() const;

    void clearUserSettings();

private:
    static const QString s_port;
    static const QString s_hostNameKey;
    static const QString s_rootPathKey;
    static const QString s_userNameKey;
    static const QString s_passwordKey;
    static const QString s_tokenKey;
    static const QString s_authTypeKey;
    static const QString s_connectionTypeKey;

    void saveValue(const QString &key, const QVariant &value = QVariant());
    QVariant value(const QString &key) const;

private:
    QSettings *m_settings;

signals:
    void portChanged();
    void hostNameChanged();
    void rootPathChanged();
    void userNameChanged();
    void passwordChanged();
    void tokenChanged();
    void authTypeChanged();
    void connectionTypeChanged();
};

#endif // SETTINGS_H
