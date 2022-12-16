// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "elementlistsortfiltermodel.h"
#include "fileandfolderlistmodel.h"

/*!
 * \brief ElementListSortFilterModel::ElementListSortFilterModel Constructor.
 * Init sort parameter and set filter role
 * \param parent
 */
ElementListSortFilterModel::ElementListSortFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    this->setFilterRole(FileAndFolderListModel::ElementsRoles::NameRole);
    setSortParameter(SortingOptions::Name);
    setOrder(Qt::AscendingOrder);
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

/*!
 * \brief ElementListSortFilterModel::lessThan overriding the method that sets the sort
 * order so that folders are displayed at the top of the lis
 * \param left
 * \param right
 * \return
 */
bool ElementListSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    auto leftData = sourceModel()
                            ->data(left, FileAndFolderListModel::ElementsRoles::ElementTypeRole)
                            .toBool();
    auto rightData = sourceModel()
                             ->data(right, FileAndFolderListModel::ElementsRoles::ElementTypeRole)
                             .toBool();

    if (leftData && !rightData)
        return m_order == Qt::AscendingOrder ? false : true;
    if (!leftData && rightData)
        return m_order == Qt::AscendingOrder ? true : false;

    return QSortFilterProxyModel::lessThan(left, right);
}

/*!
 * \brief ElementListSortFilterModel::update updating the model according to the new parameters
 */
void ElementListSortFilterModel::update()
{
    switch (m_sortParameter) {
    case SortingOptions::None:
        break;
    case SortingOptions::Date:
        setSortRole(FileAndFolderListModel::ElementsRoles::TimestampRole);
        break;
    case SortingOptions::Name:
        setSortRole(FileAndFolderListModel::ElementsRoles::NameRole);
        break;
    case SortingOptions::Size:
        setSortRole(FileAndFolderListModel::ElementsRoles::SizeRole);
        break;
    case SortingOptions::Type:
        setSortRole(FileAndFolderListModel::ElementsRoles::TypeRole);
        break;
    default:
        break;
    }

    sort(0, m_order);
}

FileAndFolderListModel *ElementListSortFilterModel::sourceModel() const
{
    return static_cast<FileAndFolderListModel *>(QSortFilterProxyModel::sourceModel());
}

void ElementListSortFilterModel::setSourceModel(FileAndFolderListModel *sourceModel)
{
    QSortFilterProxyModel::setSourceModel(static_cast<QAbstractItemModel *>(sourceModel));

    if (sourceModel != nullptr)
        connect(sourceModel, &FileAndFolderListModel::structChanged, this, [&]() {
            if (m_sortParameter == SortingOptions::Type) {
                auto tmpParameter = m_sortParameter;
                auto tmpOrder = m_order;
                m_sortParameter = SortingOptions::Name;
                m_order = Qt::AscendingOrder;
                update();
                m_sortParameter = tmpParameter;
                m_order = tmpOrder;
                update();
            }
        });
}

Qt::SortOrder ElementListSortFilterModel::order() const
{
    return m_order;
}

void ElementListSortFilterModel::setOrder(Qt::SortOrder newOrder)
{
    if (m_order == newOrder)
        return;

    m_order = newOrder;
    emit orderChanged();

    update();
}

int ElementListSortFilterModel::sortParameter() const
{
    return m_sortParameter;
}

void ElementListSortFilterModel::setSortParameter(int sortParameter)
{
    if (m_sortParameter == sortParameter)
        return;

    m_sortParameter = sortParameter;
    emit sortParameterChanged(m_sortParameter);

    update();
}
