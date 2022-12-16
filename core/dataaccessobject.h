// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef DATAACCESSOBJECT_H
#define DATAACCESSOBJECT_H

#include <QObject>
#include <QSqlDatabase>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>

#include "types/userelement.h"

/*!
 * \brief The DataAccessObject class provides work with the database
 */
class DataAccessObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QSqlDatabase db READ db NOTIFY dbChanged)
public:
    explicit DataAccessObject(const QString &dbDirPath = QStandardPaths::writableLocation(
                                      QStandardPaths::DataLocation),
                              QObject *parent = nullptr);
    QSqlDatabase db();
    void insertUsers(const UserElement &users);
    void deleteUsers(int id);
    void updateUsers(int id, QString field, QVariant value);
    bool checkAbsenceUser(const UserElement &user);
    QList<UserElement> extractAccountInformation();

signals:
    void dbChanged();
    void usersChanged();
    void deleteUsersChanged(const bool value, int id, const QString errorMsg = nullptr);

private:
    QSqlDatabase m_database;
};

#endif // DATAACCESSOBJECT_H
