// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include <QObject>

#include "operations/connectioncheckoperation.h"
#include "dataaccessobject.h"
#include "settings.h"
#include "utils/localmacros.h"
#include "types/servicesflags.h"
#include "types/fileorfolderelement.h"
#include "types/token.h"

/*!
 * \brief The ConnectionController class is a controller for the authorization page.
 */
class ConnectionController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connectionInProgress READ connectionInProgress WRITE setConnectionInProgress
                       NOTIFY connectionInProgressChanged)

    MAKE_INTERFACE_PROPERTY(Settings *, settings, Settings);
    MAKE_INTERFACE_PROPERTY(DataAccessObject *, dao, Dao);

public:
    explicit ConnectionController(QObject *parent = nullptr);

    Q_INVOKABLE void startConnection(QString address, QString username, QString password, int flag,
                                     int port = 0);
    Q_INVOKABLE void startConnection(Token token);
    Q_INVOKABLE QString errorMessage();
    Q_INVOKABLE bool emptyErrorMessage();
    Q_INVOKABLE QString nameElement(QString pathElement);
    Q_INVOKABLE QString getName(FileOrFolderElement element);
    Q_INVOKABLE QString getPath(FileOrFolderElement element);
    Q_INVOKABLE bool getIsFile(FileOrFolderElement element);
    Q_INVOKABLE QString getPathToPageByAction(int actionFlag);

    bool connectionInProgress();
    void setConnectionInProgress(bool value);
    void setCurrentSettings(QWebdav::QWebdavConnectionType connectionType, QString hostName,
                            QString rootPath, QString userName, QString password, QString token,
                            QWebdav::QWebdavAuthorizationType authType);
    QWebdav::QWebdavConnectionType getConnectionType(QUrl addressUrl);
    QString getWebDavServerBySettings();
    QString getPathCommunigate();
    QUrl getUrlAddress(QString address);

private slots:
    void handleConnectionCheckOperation(const bool success, const QString error);

signals:
    void connectionResultChanged();
    void connectionErrorChanged();
    void connectionInProgressChanged();
    void invalidToken(ServicesFlags::Flags service = ServicesFlags::YandexDisk);

private:
    ServicesFlags::Flags m_service;
    bool m_connectionInProgress;
    QString m_errorMessage;

    const QString pathCommunigate = "/WebDAV/";
    const QString pathNextcloud = "/remote.php/dav/files/%1/";
    const QString pathYandexDisk = "webdav.yandex.ru";

    void connectToYaDisk(Token token);
    void updateYandexCredentials(QVariantMap userinfo, Token token);
};

#endif // CONNECTIONCONTROLLER_H
