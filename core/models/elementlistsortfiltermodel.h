// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef ELEMENTLISTSORTFILTERMODEL_H
#define ELEMENTLISTSORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "sortingoptions.h"
#include "types/fileorfolderelement.h"

class FileAndFolderListModel;

/*!
 * \brief The ElementListSortFilterModel class provides sort filter proxy model
 * to use in qml.
 */
class ElementListSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(FileAndFolderListModel *sourceModel READ sourceModel WRITE setSourceModel NOTIFY
                       sourceModelChanged)
    Q_PROPERTY(
            int sortParameter READ sortParameter WRITE setSortParameter NOTIFY sortParameterChanged)
    Q_PROPERTY(Qt::SortOrder order READ order WRITE setOrder NOTIFY orderChanged)

    using QSortFilterProxyModel::setSourceModel;

public:
    explicit ElementListSortFilterModel(QObject *parent = nullptr);

    FileAndFolderListModel *sourceModel() const;
    int sortParameter() const;
    Qt::SortOrder order() const;
    void setOrder(Qt::SortOrder newOrder);
    void setSourceModel(FileAndFolderListModel *sourceModel);

public slots:
    void setSortParameter(int sortParameter);

signals:
    void sourceModelChanged(FileAndFolderListModel *sourceModel);
    void sortParameterChanged(int sortParemeter);
    void orderChanged();

private:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;
    void update();

private:
    int m_sortParameter;
    Qt::SortOrder m_order;
};

#endif // ELEMENTLISTSORTFILTERMODEL_H
