// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef ACCOUNTSELECTIONCONTROLLER_H
#define ACCOUNTSELECTIONCONTROLLER_H

#include "connectioncontroller.h"
#include "types/userelement.h"
#include "dataaccessobject.h"
#include <QObject>

class RemoteServiceModel;

/*!
 * \brief The AccountSelectionController class account selection page controller
 */
class AccountSelectionController : public ConnectionController
{
    Q_OBJECT
    Q_PROPERTY(RemoteServiceModel *remoteServiceModel READ remoteServiceModel NOTIFY
                       remoteServiceModelChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage);
    Q_PROPERTY(QString msgTitle READ msgTitle);

public:
    explicit AccountSelectionController(QObject *parent = nullptr);

    Q_INVOKABLE void updateModelData();
    Q_INVOKABLE void deleteAccount(int id);
    RemoteServiceModel *remoteServiceModel();
    QString errorMessage();
    QString msgTitle();

private slots:
    void handleDeleteUser(bool success, int id, QString userName);

private:
    RemoteServiceModel *m_remoteServiceModel;
    QString m_errorMessage;
    QString m_msgTitle;

signals:
    void remoteServiceModelChanged();
    void operationSuccess();
    void operationError();
};

#endif // ACCOUNTSELECTIONCONTROLLER_H
