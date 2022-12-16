// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef MIGRATIONS_H
#define MIGRATIONS_H

#include <QObject>
#include <QSqlDatabase>
#include <QFileInfo>

/*!
 * \brief The Migrations class checks for a versioned table,
 * creates it if necessary, and then performs migration changes
 * according to the database version and updates the versioned table.
 */
class Migrations : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSqlDatabase db READ db WRITE setDb NOTIFY dbChanged())

public:
    explicit Migrations(QObject *parent = nullptr) : QObject(parent) { }

    Q_INVOKABLE bool performMigration();

    void setDb(QSqlDatabase db);
    QSqlDatabase db();

private:
    bool createVersionTable();
    bool setSchemaVersion(QString version);
    bool initializeMigrationTable();
    bool openAndReadSqlFile(QFileInfo fileInfo);
    bool applySqlFile(QStringList scriptQueries, QFileInfo fileInfo);
    bool enshureVersionTableExist();
    QString extractDataFromVersionTable();

    QSqlDatabase m_db;

signals:
    void dbChanged();
};

Q_DECLARE_METATYPE(QSqlDatabase) // For using QSqlDatabase as property

#endif // MIGRATIONS_H
