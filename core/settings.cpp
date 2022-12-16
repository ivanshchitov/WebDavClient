// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "settings.h"

// Settings keys
const QString Settings::s_port = "port";
const QString Settings::s_hostNameKey = "host_name";
const QString Settings::s_rootPathKey = "root_path";
const QString Settings::s_userNameKey = "user_name";
const QString Settings::s_passwordKey = "password";
const QString Settings::s_tokenKey = "token";
const QString Settings::s_authTypeKey = "auth_type";
const QString Settings::s_connectionTypeKey = "connection_type";

/*!
 * \brief Settings::Settings
 * \param confFilePath
 * \param parent
 */
Settings::Settings(const QString &confFilePath, QObject *parent) : QObject(parent)
{
    if (confFilePath.isEmpty())
        m_settings = new QSettings(this);
    else
        m_settings = new QSettings(confFilePath, QSettings::IniFormat, this);
}

/*!
 * \brief Clears user settings.
 */
void Settings::clearUserSettings()
{
    m_settings->remove(s_hostNameKey);
    m_settings->remove(s_rootPathKey);
    m_settings->remove(s_userNameKey);
    m_settings->remove(s_passwordKey);
    m_settings->remove(s_userNameKey);
    m_settings->remove(s_tokenKey);
    m_settings->remove(s_authTypeKey);
    m_settings->remove(s_port);
    m_settings->remove(s_connectionTypeKey);
    emit hostNameChanged();
    emit rootPathChanged();
    emit userNameChanged();
    emit passwordChanged();
    emit userNameChanged();
    emit tokenChanged();
    emit authTypeChanged();
    emit portChanged();
    emit connectionTypeChanged();
}

/*!
 * \brief Saves the current portal address URL to perform requests.
 * \param hostName Portal address URL to save.
 */
void Settings::saveHostName(const QString &hostName)
{
    saveValue(s_hostNameKey, hostName);
    emit hostNameChanged();
}

/*!
 * \brief Saves the root path of the authorized user
 * \param rootPath Root path to save.
 */
void Settings::saveRootPath(const QString &rootPath)
{
    saveValue(s_rootPathKey, rootPath);
    emit rootPathChanged();
}

/*!
 * \brief Saves the user name used for the authorization procedure.
 * \param userName User name to save.
 */
void Settings::saveUserName(const QString &userName)
{
    saveValue(s_userNameKey, userName);
    emit userNameChanged();
}

/*!
 * \brief Saves the user password used for the authorization procedure.
 * Password will be converted to base64 and to hex before saving.
 * \param password User password to save.
 */
void Settings::savePassword(const QString &password)
{
    saveValue(s_passwordKey, password.toUtf8().toBase64().toHex());
    emit passwordChanged();
}

/*!
 * \brief Saves oauth token for authorized user.
 * \param token OAuth token.
 */
void Settings::saveToken(const QString &token)
{
    saveValue(s_tokenKey, token);
    emit tokenChanged();
}

/*!
 * \brief Saves the authorization type of the authorized user
 * \param authorization type
 */
void Settings::saveAuthType(QWebdav::QWebdavAuthorizationType authType)
{
    saveValue(s_authTypeKey, authType);
    emit authTypeChanged();
}

/*!
 * \brief Saves the port of the authorized user
 * \param port
 */
void Settings::savePort(const QString port)
{
    saveValue(s_port, port);
    emit portChanged();
}

/*!
 * \brief Saves the connection type of the authorized user
 * \param connection type
 */
void Settings::saveConnectionType(QWebdav::QWebdavConnectionType connectionType)
{
    saveValue(s_connectionTypeKey, connectionType);
    emit connectionTypeChanged();
}

/*!
 * \return Portal address used to perform requests from settings.
 */
QString Settings::hostName() const
{
    return value(s_hostNameKey).toString();
}

/*!
 * \return Root path used to perform requests from settings
 */
QString Settings::rootPath() const
{
    return value(s_rootPathKey).toString();
}

/*!
 * \return User name from settings.
 */
QString Settings::userName() const
{
    return value(s_userNameKey).toString();
}

/*!
 * \return User password from settings.
 */
QString Settings::password() const
{
    return QByteArray::fromBase64(QByteArray::fromHex(value(s_passwordKey).toString().toUtf8()));
}

/*!
 * \return OAuth token from settings.
 */
QString Settings::token() const
{
    return value(s_tokenKey).toString();
}

/*!
 * \return Port from settings.
 */
QString Settings::port() const
{
    return value(s_port).toString();
}

/*!
 * \return authorization type
 */
QWebdav::QWebdavAuthorizationType Settings::authType() const
{
    if (value(s_authTypeKey).toInt())
        return QWebdav::OAuth;
    return QWebdav::Basic;
}

/*!
 * \return connection type
 */
QWebdav::QWebdavConnectionType Settings::connectionType() const
{
    if (value(s_connectionTypeKey).toInt() == 1)
        return QWebdav::HTTP;
    else if (value(s_connectionTypeKey).toInt() == 2)
        return QWebdav::HTTPS;
    return {};
}

/*!
 * \brief Saves the given value with into settings by the given key.
 * \param key Setting key.
 * \param value Value to save.
 */
void Settings::saveValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
    m_settings->sync();
}

/*!
 * \brief Retrieves the setting value by the given key.
 * \param key Setting key.
 * \return Setting value as QVariant.
 */
QVariant Settings::value(const QString &key) const
{
    if (m_settings == nullptr)
        return QVariant();
    m_settings->sync();
    return m_settings->value(key, "");
}
