// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "remoteservicemodel.h"

RemoteServiceModel::RemoteServiceModel(QObject *parent) : QAbstractListModel(parent) { }

/*!
 * \brief RemoteServiceModel::rowCount
 * \param parent
 * \return row count in model
 */
int RemoteServiceModel::rowCount(const QModelIndex &) const
{
    return m_elements.size();
}

/*!
 * \brief RemoteServiceModel::data using directories and files lists.
 * \param index
 * \param role
 * \return data by role
 */
QVariant RemoteServiceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > m_elements.size())
        return {};

    auto element = m_elements.at(index.row());
    switch (role) {
    case IdRole:
        return QVariant::fromValue(element.id);
        ;
    case PortalAddressRole:
        return QVariant::fromValue(element.portalAddress);
    case UserNameRole:
        return QVariant::fromValue(element.userName);
    case PasswordRole:
        return QVariant::fromValue(element.password);
    case ServiceRole:
        return QVariant::fromValue(element.service);
    case BusyRole:
        return QVariant::fromValue(element.busy);
    case TokenRole:
        return QVariant::fromValue(element.token);
    default:
        return {};
    }
    return {};
}

/*!
 * \brief RemoteServiceModel::roleNames
 * \return string view of a roles
 */
QHash<int, QByteArray> RemoteServiceModel::roleNames() const
{
    const static QHash<int, QByteArray> rolesHash{
        { IdRole, "id" },
        { PortalAddressRole, "portalAddress" },
        { UserNameRole, "userName" },
        { PasswordRole, "password" },
        { ServiceRole, "service" },
        { BusyRole, "busy" },
        { TokenRole, "token" },
    };

    return rolesHash;
}

/*!
 * \brief RemoteServiceModel::setData set new data by index and role. We can change only
 * selected field
 * \param index
 * \param value
 * \param role
 * \return true if set new data is success
 */
bool RemoteServiceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() > m_elements.size())
        return false;

    if (role < Qt::UserRole)
        return QAbstractListModel::setData(index, value, role);

    switch (role) {
    case IdRole:
        m_elements[index.row()].id = value.toInt();
        break;
    case PortalAddressRole:
        m_elements[index.row()].portalAddress = value.toString();
        break;
    case UserNameRole:
        m_elements[index.row()].userName = value.toString();
        break;
    case PasswordRole:
        m_elements[index.row()].password = value.toString();
        break;
    case ServiceRole:
        m_elements[index.row()].service = value.toString();
        break;
    case BusyRole:
        m_elements[index.row()].busy = value.toBool();
        break;
    case TokenRole:
        m_elements[index.row()].token = value.value<Token>();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

/*!
 * \brief RemoteServiceModel::setElementsForModel setting elements for model.
 * \param list List with elements.
 */
void RemoteServiceModel::setElementsForModel(QList<UserElement> list)
{
    beginResetModel();
    m_elements.clear();
    m_elements = list;
    endResetModel();
}

/*!
 * \brief RemoteServiceModel::setBusy sets the busy value by the id
 * \param id account id
 * \param value
 */
void RemoteServiceModel::setBusy(int id, bool value)
{
    int elementIndex = 0;
    for (UserElement item : m_elements) {
        if (item.id == id) {
            QVariant val(value);
            setData(index(elementIndex, 0), val, BusyRole);
            break;
        }
        elementIndex++;
    }
}

/*!
 * \brief RemoteServiceModel::deleteById
 * \param id the id of the account being deleted
 */
void RemoteServiceModel::deleteById(int id)
{
    QMutableListIterator<UserElement> list(m_elements);
    int elementIndex = 0;
    while (list.hasNext()) {
        UserElement currentValue = list.next();
        if (currentValue.id == id) {
            beginRemoveRows(QModelIndex(), elementIndex, elementIndex);
            list.remove();
            endRemoveRows();
            break;
        } else
            ++elementIndex;
    }
}

/*!
 * \brief RemoteServiceModel::userNameById returns the name by the passed id
 * \param id the id of the account being deleted
 * \return account login
 */
QString RemoteServiceModel::userNameById(int id)
{
    int elementIndex = 0;
    for (UserElement item : m_elements) {
        if (item.id == id)
            return item.userName;
        elementIndex++;
    }
    return {};
}
