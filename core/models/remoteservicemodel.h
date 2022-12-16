// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef REMOTESERVICEMODEL_H
#define REMOTESERVICEMODEL_H

#include <QAbstractListModel>
#include "types/userelement.h"

/*!
 * \brief The RemoteServiceModel class the model class responsible for displaying the list of
 * accounts
 */
class RemoteServiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        IdRole = Qt::UserRole + 1,
        PortalAddressRole,
        UserNameRole,
        PasswordRole,
        ServiceRole,
        BusyRole,
        TokenRole,
    };
    Q_ENUM(UserRoles)

    explicit RemoteServiceModel(QObject *parent = nullptr);

    void setElementsForModel(QList<UserElement> list);
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    void setBusy(int id, bool value);
    void deleteById(int id);
    QString userNameById(int id);

private:
    QList<UserElement> m_elements;
};

#endif // REMOTESERVICEMODEL_H
