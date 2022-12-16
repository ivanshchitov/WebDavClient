// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "fileandfolderlistmodel.h"

/*!
 * \brief FileAndFolderListModel::FileAndFolderListModel constructor
 * \param parent
 */
FileAndFolderListModel::FileAndFolderListModel(QObject *parent) : QAbstractListModel(parent) { }

/*!
 * \brief FileAndFolderListModel::rowCount
 * \param parent
 * \return row count in model
 */
int FileAndFolderListModel::rowCount(const QModelIndex &) const
{
    return m_elements.size();
}

/*!
 * \brief FileAndFolderListModel::data using directories and files lists.
 * \param index
 * \param role
 * \return data by role
 */
QVariant FileAndFolderListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > m_elements.size())
        return {};

    auto file = m_elements.at(index.row());
    switch (role) {
    case NameRole:

        return QVariant::fromValue(file.name);
        ;
    case TimestampRole:
        return QVariant::fromValue(file.timestamp);
    case ElementTypeRole:
        return QVariant::fromValue(file.is_file);
    case BusyRole:
        return QVariant::fromValue(file.busy);
    case SizeRole:
        return QVariant::fromValue(file.size);
    case TypeRole:
        return QVariant::fromValue(file.name.split(".").last());
    case SelectedRole:
        return QVariant::fromValue(file.selected);
    case PathRole:
        return QVariant::fromValue(file.path);
    default:
        return {};
    }
    return {};
}

/*!
 * \brief FileAndFolderListModel::roleNames
 * \return string view of a roles
 */
QHash<int, QByteArray> FileAndFolderListModel::roleNames() const
{
    const static QHash<int, QByteArray> rolesHash{
        { NameRole, "name" },           { TimestampRole, "timestamp" },
        { ElementTypeRole, "is_file" }, { BusyRole, "busy" },
        { SizeRole, "size" },           { TypeRole, "type" },
        { SelectedRole, "selected" },   { PathRole, "path" },
    };

    return rolesHash;
}

/*!
 * \brief FileAndFolderListModel::setData set new data by index and role. We can change only
 * selected field
 * \param index
 * \param value
 * \param role
 * \return true if set new data is success
 */
bool FileAndFolderListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() > m_elements.size())
        return false;

    if (role < Qt::UserRole)
        return QAbstractListModel::setData(index, value, role);

    switch (role) {
    case NameRole:
        m_elements[index.row()].name = value.toString();
        break;
    case TimestampRole:
        return false;
    case ElementTypeRole:
        return false;
    case BusyRole:
        m_elements[index.row()].busy = value.toBool();
        break;
    case SizeRole:
        return false;
    case TypeRole:
        return false;
    case SelectedRole:
        return false;
    case PathRole:
        m_elements[index.row()].path = value.toString();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

/*!
 * \brief FileAndFolderListModel::getElement returns data for the specified path
 * \param path
 * \return data
 */
FileOrFolderElement FileAndFolderListModel::getElement(QString path) const
{
    for (FileOrFolderElement item : m_elements) {
        if (item.path == path)
            return item;
    }
    return {};
}

/*!
 * \brief FileAndFolderListModel::setElementsForModel setting elements for model.
 * \param list List with elements.
 */
void FileAndFolderListModel::setElementsForModel(QList<FileOrFolderElement> list)
{
    QMutableListIterator<FileOrFolderElement> iterator(m_elements);
    int elementIndex = 0;
    while (iterator.hasNext()) {
        if (!list.contains(iterator.next())) {
            beginRemoveRows(QModelIndex(), elementIndex, elementIndex);
            iterator.remove();
            endRemoveRows();
        } else {
            ++elementIndex;
        }
    }

    for (const auto &newElement : list) {
        if (!m_elements.contains(newElement)) {
            beginInsertRows(QModelIndex(), m_elements.size(), m_elements.size());
            m_elements.append(newElement);
            endInsertRows();
        }
    }
}

/*!
 * \brief FileAndFolderListModel::setBusy sets the busy value by the path
 * \param path file/directory path
 * \param value
 */
void FileAndFolderListModel::setBusy(QString path, bool value)
{
    int elementIndex = 0;
    for (FileOrFolderElement item : m_elements) {
        if (item.path == path) {
            QVariant val(value);
            setData(index(elementIndex, 0), val, BusyRole);
            break;
        }
        elementIndex++;
    }
}

/*!
 * \brief FileAndFolderListModel::deleteByPath deletes a model element by the path
 * \param path the path of the file/directory being deleted
 */
void FileAndFolderListModel::deleteByPath(QString path)
{
    QMutableListIterator<FileOrFolderElement> list(m_elements);
    int elementIndex = 0;
    while (list.hasNext()) {
        FileOrFolderElement currentValue = list.next();
        if (currentValue.path == path) {
            beginRemoveRows(QModelIndex(), elementIndex, elementIndex);
            list.remove();
            endRemoveRows();
            break;
        } else
            ++elementIndex;
    }
}

/*!
 * \brief FileAndFolderListModel::renameByPath renames a model element by the path
 * \param path the path of the file/directory being renamed
 * \param newName new name of the file/directory
 */
void FileAndFolderListModel::renameByPath(QString path, QString newName)
{
    int elementIndex = 0;
    for (FileOrFolderElement item : m_elements) {
        if (item.path == path) {
            QVariant val(newName);
            setData(index(elementIndex, 0), val, NameRole);
            break;
        }
        elementIndex++;
    }
}

/*!
 * \brief FileAndFolderListModel::searchByPath search a model element by the path
 * \param path path to the file/directory being searched
 * \return true, if file/directory already exist
 */
bool FileAndFolderListModel::isPathExist(QString path)
{
    for (FileOrFolderElement item : m_elements) {
        if (item.path == path)
            return true;
    }
    return false;
}

/*!
 * \brief FileAndFolderListModel::clear remove all rows from model
 */
void FileAndFolderListModel::clear()
{
    beginResetModel();
    m_elements.clear();
    endResetModel();
}
