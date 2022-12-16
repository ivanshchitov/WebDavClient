// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FILEANDFOLDERLISTMODEL_H
#define FILEANDFOLDERLISTMODEL_H

#include <QAbstractListModel>
#include <types/fileorfolderelement.h>

/*!
 * \brief The FileAndFolderListModel class of the model responsible
 * for displaying the list of files and folders
 */
class FileAndFolderListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ElementsRoles {
        NameRole = Qt::UserRole + 1,
        TimestampRole,
        ElementTypeRole,
        BusyRole,
        SizeRole,
        TypeRole,
        SelectedRole,
        PathRole,
    };
    Q_ENUM(ElementsRoles)

    explicit FileAndFolderListModel(QObject *parent = nullptr);

    void setElementsForModel(QList<FileOrFolderElement> list = QList<FileOrFolderElement>());
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    FileOrFolderElement getElement(QString path) const;

    void setBusy(QString path, bool value);
    void deleteByPath(QString path);
    void renameByPath(QString path, QString newName);
    void clear();
    bool isPathExist(QString path);

signals:
    void structChanged();

private:
    QList<FileOrFolderElement> m_elements;
};

#endif // FILEANDFOLDERLISTMODEL_H
