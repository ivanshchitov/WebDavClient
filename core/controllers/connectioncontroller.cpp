// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "operations/getuserinfooperation.h"
#include "operations/connectioncheckoperation.h"
#include "../types/actionflags.h"

#include "connectioncontroller.h"
#include "controllers/oauthprovider.h"

/*!
 * \brief ConnectionController::ConnectionController constructor
 * \param parent
 */
ConnectionController::ConnectionController(QObject *parent)
    : QObject(parent), m_settings(nullptr), m_dao(nullptr)
{
}

/*!
 * \brief ConnectionController::startConnection Establishes a connection
 * to the specified service, address, username and password
 * \param address portal address
 * \param username login
 * \param password password
 */
void ConnectionController::startConnection(QString address, QString username, QString password,
                                           int servicesFlag, int port)
{
    m_errorMessage.clear();
    setConnectionInProgress(true);
    m_settings->clearUserSettings();

    switch (servicesFlag) {
    case ServicesFlags::NextCloud:
        setCurrentSettings(QWebdav::HTTPS, address, pathNextcloud.arg(username), username, password,
                           "", QWebdav::Basic);
        m_service = ServicesFlags::NextCloud;
        break;
    case ServicesFlags::Communigate:
        setCurrentSettings(QWebdav::HTTPS, address, pathCommunigate, username, password, "",
                           QWebdav::Basic);
        m_settings->savePort(QString::number(port));
        m_service = ServicesFlags::Communigate;
        break;
    case ServicesFlags::WebDavServer:
        QUrl addressUrl(getUrlAddress(address));
        setCurrentSettings(getConnectionType(addressUrl), addressUrl.host(), addressUrl.path(),
                           username, password, "", QWebdav::Basic);
        m_service = ServicesFlags::WebDavServer;
        break;
    }

    auto *connectionCheckOperation = new ConnectionCheckOperation(m_settings);
    connect(connectionCheckOperation, &ConnectionCheckOperation::operationFinished, this,
            &ConnectionController::handleConnectionCheckOperation);
    connectionCheckOperation->checkConnection();
}

/*!
 * \brief Handles response from ConnectionCheckOperation
 * \param success True if operation was successful
 * \param error Error string
 */
void ConnectionController::handleConnectionCheckOperation(const bool success, const QString error)
{
    setConnectionInProgress(false);
    if (!success) {
        m_errorMessage = error;
        emit connectionErrorChanged();
    } else {
        QString hostName = m_settings->hostName();
        if (m_service == ServicesFlags::WebDavServer)
            hostName = getWebDavServerBySettings();
        UserElement user(hostName, m_settings->userName(), m_settings->password(),
                         ServicesFlags::servicesFlagToString(m_service), false);
        if (m_dao->checkAbsenceUser(user))
            m_dao->insertUsers(user);
        emit connectionResultChanged();
    }
}

/*!
 * \brief ConnectionController::startConnection Configure webdav connection using OAuth token.
 * \param address nextcloud portal address
 * \param username login
 * \param password password
 */
void ConnectionController::startConnection(Token token)
{
    m_settings->clearUserSettings();
    if (QDateTime::currentDateTime() >= token.created().addSecs(token.expiresIn())) {
        emit invalidToken();
    } else {
        setConnectionInProgress(true);
        OAuthProvider *oauth = new OAuthProvider();
        oauth->setPortalUrl(QUrl("https://oauth.yandex.ru/"));
        oauth->setClientId(CLIENT_ID);
        oauth->setClientSecret(CLIENT_SECRET);
        connect(oauth, &OAuthProvider::tokenChanged, this, [=](Token newToken) {
            connectToYaDisk(newToken);
            oauth->deleteLater();
        });
        connect(oauth, &OAuthProvider::errorOccured, this, [=](QString, QString errorDescription) {
            setConnectionInProgress(false);
            if (errorDescription == "expired_token") {
                emit invalidToken();
            } else {
                m_errorMessage = errorDescription;
                emit connectionErrorChanged();
            }
            oauth->deleteLater();
        });
        oauth->refreshAccessToken(token.refreshToken());
    }
}

/*!
 * \brief ConnectionController::errorMessage return error message
 * \return error message
 */
QString ConnectionController::errorMessage()
{
    return m_errorMessage;
}

/*!
 * \brief ConnectionController::emptyErrorMessage check that there is no error message
 * \return a bool value that defines the emptiness of the error message
 */
bool ConnectionController::emptyErrorMessage()
{
    return m_errorMessage.isEmpty();
}

/*!
 * \brief ConnectionController::setConnectionInProgress sets the connection progress status
 * \param value progress status
 */
void ConnectionController::setConnectionInProgress(bool value)
{
    m_connectionInProgress = value;
    emit connectionInProgressChanged();
}

/*!
 * \brief ConnectionController::connectionInProgress
 * \return progress status
 */
bool ConnectionController::connectionInProgress()
{
    return m_connectionInProgress;
}

/*!
 * \brief FolderPageController::nameElement extracts the name from the path
 * \param pathElement element path
 * \return element name
 */
QString ConnectionController::nameElement(QString pathElement)
{
    QStringList list = pathElement.split("/");
    if (list.last().isEmpty())
        return list[list.size() - 2];
    return list.last();
}

/*!
 * \brief ConnectionController::getName returns the name of the model element
 * \param element model element
 * \return name
 */
QString ConnectionController::getName(FileOrFolderElement element)
{
    return element.name;
}

/*!
 * \brief ConnectionController::getPath returns the path of the model element
 * \param element model element
 * \return name
 */
QString ConnectionController::getPath(FileOrFolderElement element)
{
    return element.path;
}

/*!
 * \brief ConnectionController::getIsFile returns the isFile of the model element
 * \param element model element
 * \return isFile
 */
bool ConnectionController::getIsFile(FileOrFolderElement element)
{
    return element.is_file;
}

/*!
 * \brief ConnectionController::getUrlAddress returns the url link created from the address string
 * \param address
 * \return link url
 */
QUrl ConnectionController::getUrlAddress(QString address)
{
    if (QUrl(address).scheme().isEmpty())
        return QUrl(QString("https://%1").arg(address));
    return QUrl(address);
}

/*!
 * \brief ConnectionController::getConnectionType returns the connection type from the string QUrl
 * \param addressUrl QUrl string
 * \return connection type
 */
QWebdav::QWebdavConnectionType ConnectionController::getConnectionType(QUrl addressUrl)
{
    if (QString::compare(addressUrl.scheme(), "http", Qt::CaseInsensitive) == 0)
        return QWebdav::HTTP;
    else if (QString::compare(addressUrl.scheme(), "https", Qt::CaseInsensitive) == 0)
        return QWebdav::HTTPS;
    return {};
}

/*!
 * \brief ConnectionController::getWebDavServerBySettings composes the full webdav address
 * according to the data from the settings
 * \return webdav address
 */
QString ConnectionController::getWebDavServerBySettings()
{
    if (m_settings->connectionType() == 1)
        return QStringLiteral("http://%2%3").arg(m_settings->hostName(), m_settings->rootPath());
    else
        return QStringLiteral("https://%2%3").arg(m_settings->hostName(), m_settings->rootPath());
}

/*!
 * \brief ConnectionController::setCurrentSettings sets the correct settings
 * \param connectionType connection type
 * \param hostName portal address
 * \param rootPath connection path
 * \param userName user name
 * \param password
 * \param authType authorization type
 */
void ConnectionController::setCurrentSettings(QWebdav::QWebdavConnectionType connectionType,
                                              QString hostName, QString rootPath, QString userName,
                                              QString password, QString token,
                                              QWebdav::QWebdavAuthorizationType authType)
{
    m_settings->savePort(QString(QStringLiteral("8443")));
    m_settings->saveConnectionType(connectionType);
    m_settings->saveHostName(hostName);
    if (!rootPath.endsWith("/"))
        rootPath += "/";
    m_settings->saveRootPath(rootPath);
    m_settings->saveAuthType(authType);
    if (authType == QWebdav::QWebdavAuthorizationType::Basic) {
        m_settings->saveUserName(userName);
        m_settings->savePassword(password);
    } else
        m_settings->saveToken(token);

    qDebug() << "MY_LOG: " << hostName << " " << rootPath;
}

/*!
 * \brief ConnectionController::getPathToPageByAction gets file path by action flag
 * \param actionFlag Flag with current operation.
 * \return Path to the file
 */
QString ConnectionController::getPathToPageByAction(int actionFlag)
{
    switch (actionFlag) {
    case (ActionFlags::Flags::Upload):
        return QStringLiteral("UploadLoadingPage.qml");
    case (ActionFlags::Flags::Download):
        return QStringLiteral("DownloadLoadingPage.qml");
    case (ActionFlags::Flags::MoveToDevice):
        return QStringLiteral("MoveToDeviceLoadingPage.qml");
    }
    return {};
}

/*!
 * \brief ConnectionController::getPathCommunigate returns the root path for Communigate
 * \return root path for Communigate
 */
QString ConnectionController::getPathCommunigate()
{
    return pathCommunigate;
}

/*!
 * \brief Check connection to Yandex.Disk using userinfo query.
 * \param token Access token.
 */
void ConnectionController::connectToYaDisk(Token token)
{
    setConnectionInProgress(true);
    setCurrentSettings(QWebdav::HTTPS, pathYandexDisk, "/", "", "", token.accessToken(),
                       QWebdav::OAuth);
    GetUserInfoOperation *op = new GetUserInfoOperation(m_settings);
    connect(op, &GetUserInfoOperation::operationFinished, this,
            [=](const QVariantMap userinfo, const QNetworkReply::NetworkError error,
                const QString errorString) {
                setConnectionInProgress(false);
                if (userinfo.isEmpty()) {
                    if (error == QNetworkReply::OperationCanceledError) {
                        emit invalidToken();
                    } else {
                        m_errorMessage = errorString;
                        emit connectionErrorChanged();
                    }
                } else {
                    updateYandexCredentials(userinfo, token);
                    emit connectionResultChanged();
                }
                op->deleteLater();
            });
    op->run();
}

/*!
 * \brief Update creadentials of authorized account.
 * \param userinfo User information
 * \param token Token information
 */
void ConnectionController::updateYandexCredentials(QVariantMap userinfo, Token token)
{
    QList<UserElement> accounts = m_dao->extractAccountInformation();
    auto searchResult = std::find_if(accounts.cbegin(), accounts.cend(), [userinfo](UserElement e) {
        return userinfo["login"] == e.userName;
    });
    if (searchResult == accounts.cend()) {
        UserElement user(pathYandexDisk, userinfo["login"].toString(), {},
                         ServicesFlags::servicesFlagToString(ServicesFlags::YandexDisk), false,
                         token);
        this->m_dao->insertUsers(user);
    } else if (searchResult->token.serialize() != token.serialize()) {
        m_dao->updateUsers(searchResult->id, "token", token.serialize());
    }
}
