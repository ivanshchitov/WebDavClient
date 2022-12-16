// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

#include "dataaccessobject.h"

/*!
 * \brief Constructor initilizes the database inside a directory by the given path.
 * \param dbDirPath Path to a directory with the database sqlite file.
 * \param parent The parent of this object.
 */
DataAccessObject::DataAccessObject(const QString &dbDirPath, QObject *parent) : QObject(parent)
{
    qInfo() << "Database initialization.";
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    if (!QDir().exists(dbDirPath))
        QDir().mkpath(dbDirPath);
    m_database.setDatabaseName(dbDirPath + "/DataBase.sqlite");
    qInfo() << QString("Try to open database %1").arg(m_database.databaseName());
    if (m_database.open()) {
        qInfo() << "Database is opened successfully.";

        QSqlQuery query;
        query.prepare("SELECT * FROM sqlite_master");
        if (query.exec()) {
            QString result;
            qInfo() << "DB contains tables:";
            while (query.next()) {
                if (query.value("type") == "table") {
                    result = query.value("name").toString();
                    qInfo() << "\t" << result;
                }
            }
        }
    } else
        qFatal("Database isn't opened: %s", +m_database.lastError().text().toUtf8().data());
}

/*!
 * \brief DataAccessObject::insertUsers inserts a new account into the database
 * \param user new account
 */
void DataAccessObject::insertUsers(const UserElement &user)
{
    QSqlQuery query;
    QString queryString = "INSERT OR REPLACE INTO users(portalAddress, userName, "
                          "password, service, busy, token, tokenTimeReceipt, tokenLifetime) VALUES "
                          "(:portalAddress, :userName, :password, :service, :busy, "
                          ":token, :tokenTimeReceipt, :tokenLifetime)";
    query.prepare(queryString);
    query.bindValue(":portalAddress", user.portalAddress);
    query.bindValue(":userName", user.userName);
    query.bindValue(":password", user.password);
    query.bindValue(":service", user.service);
    query.bindValue(":busy", user.busy);
    query.bindValue(":token", user.token.serialize());
    if (query.exec()) {
        qInfo() << "User are inserted";
        emit usersChanged();
    } else {
        qFatal("%s", QString("Insert failed: " + query.lastError().text()).toUtf8().data());
    }
}

/*!
 * \brief DataAccessObject::checkAbsenceUser checks if there is a
 * transferred account in the database
 * \param user account
 * \return check result
 */
bool DataAccessObject::checkAbsenceUser(const UserElement &user)
{
    QSqlQuery query;
    QString queryString = "SELECT COUNT(*) FROM users WHERE (portalAddress = ? AND userName = ? );";
    query.prepare(queryString);
    query.addBindValue(user.portalAddress);
    query.addBindValue(user.userName);
    if (query.exec() && query.next()) {
        if (!query.value(0).toInt())
            return true;
    } else {
        qFatal("%s", QString("Check failed: " + query.lastError().text()).toUtf8().data());
    }
    return false;
}

/*!
 * \brief DataAccessObject::extractAccountInformation retrieves account data from the database
 * \return list of accounts
 */
QList<UserElement> DataAccessObject::extractAccountInformation()
{
    QString queryString = QStringLiteral("SELECT * FROM users");
    QSqlQuery query;
    query.prepare(queryString);

    if (!query.exec()) {
        qWarning() << QString("%1").arg(query.lastError().text());
        return {};
    }

    QList<UserElement> result;
    while (query.next()) {
        UserElement element;
        element.id = query.value(QStringLiteral("id")).toInt();
        element.portalAddress = query.value(QStringLiteral("portalAddress")).toString();
        element.userName = query.value(QStringLiteral("userName")).toString();
        element.password = query.value(QStringLiteral("password")).toString();
        element.service = query.value(QStringLiteral("service")).toString();
        element.busy = query.value(QStringLiteral("busy")).toBool();
        element.token = Token(query.value(QStringLiteral("token")).toByteArray());
        result.append(element);
    }
    qInfo() << "User are selected";
    emit usersChanged();
    return result;
}

/*!
 * \brief DataAccessObject::deleteUsers deletes an account from the database
 * \param id
 */
void DataAccessObject::deleteUsers(int id)
{
    QSqlQuery query;
    QString queryString = "DELETE FROM users WHERE id = ? ;";
    query.prepare(queryString);
    query.addBindValue(id);
    if (!query.exec()) {
        qWarning() << "Delete failed: " + query.lastError().text();
        emit deleteUsersChanged(false, id, query.lastError().text());
    } else {
        qInfo() << "User are deleted";
        emit deleteUsersChanged(true, id);
    }
}

/*!
 * \brief Update a single field of a record with specified id
 * \param id Record id
 * \param field Updated field
 * \param value Field value
 */
void DataAccessObject::updateUsers(int id, QString field, QVariant value)
{
    QSqlQuery query;
    query.prepare(QString("UPDATE users SET %1=:value WHERE id=:id;").arg(field));
    query.bindValue(":id", id);
    query.bindValue(":value", value);
    if (query.exec()) {
        qInfo() << QString("User %1 updated").arg(id);
        emit usersChanged();
    } else {
        qFatal("%s", QString("Update failed: " + query.lastError().text()).toUtf8().data());
    }
}

QSqlDatabase DataAccessObject::db()
{
    return m_database;
}
