// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>
#include <QDir>

#include "migrations.h"

/*!
 * \brief Migrations::createVersionTable creates a table with the database version
 * \return A boolean value indicating the success or failure of the executed request
 */
bool Migrations::createVersionTable()
{
    QSqlQuery query(m_db);
    if (!query.exec("CREATE TABLE version_info (version VARCHAR(200));")) {
        qFatal("Migrations: Error in creating the version table: %s",
               qPrintable(query.lastError().text()));
        return false;
    }
    return true;
}

/*!
 * \brief Migrations::setSchemaVersion updates the table with the database version
 * \param version Name database version
 * \return A boolean value indicating the success or failure of the executed request
 */
bool Migrations::setSchemaVersion(QString version)
{
    QSqlQuery query(m_db);
    if (!query.exec("DELETE FROM version_info;")) {
        qFatal("Migrations: Error in clearing the version table: %s",
               qPrintable(query.lastError().text()));
        return false;
    }
    query.prepare("INSERT INTO version_info (version) VALUES (:version);");
    query.bindValue(":version", version);
    if (!query.exec()) {
        qFatal("Migrations: Error in filling in the version table: %s",
               qPrintable(query.lastError().text()));
        return false;
    }
    return true;
}

/*!
 * \brief Migrations::initializeMigrationTable  creates and populates a database migration table
 * \return A boolean value indicating the success or failure of the executed request
 */
bool Migrations::initializeMigrationTable()
{
    if (!this->createVersionTable()) {
        qFatal("Migrations: Initialization of the table with the version is broken at the stage of "
               "creating the table.");
        return false;
    }
    if (!this->setSchemaVersion("0")) {
        qFatal("Migrations: Initialization of the table with the version is interrupted at the "
               "stage of filling in the table.");
        return false;
    }
    return true;
}

/*!
 * \brief Migrations::openAndReadSqlFile opens and reads the sql file
 * \param fileInfo Information about the migration file being executed
 * \return A value of the applySqlFile function
 */
bool Migrations::openAndReadSqlFile(QFileInfo fileInfo)
{
    QFile scriptFile(fileInfo.filePath());
    if (!(scriptFile.open(QIODevice::ReadOnly))) {
        qFatal("Migrations: An error occurred when opening the file");
        return false;
    }
    QStringList scriptQueries = QTextStream(&scriptFile).readAll().split(';');
    return applySqlFile(scriptQueries, fileInfo);
}

/*!
 * \brief Migrations::applySqlFile apply the sql file
 * \param scriptQueries List of sql queries of the migration file
 * \param fileInfo Information about the migration file being executed
 * \return A boolean value indicating the success or failure of the executed request
 */
bool Migrations::applySqlFile(QStringList scriptQueries, QFileInfo fileInfo)
{
    QSqlQuery query(m_db);
    query.exec("BEGIN TRANSACTION;");
    foreach (QString queryTxt, scriptQueries) {
        if (queryTxt.trimmed().isEmpty()) {
            continue;
        }
        if (!query.exec(queryTxt)) {
            qFatal("Migrations: Error in the file '%s', sql-query '%s', type of error: %s",
                   qPrintable(fileInfo.fileName()), qPrintable(queryTxt),
                   qPrintable(query.lastError().text()));
            query.finish();
            query.exec("ROLLBACK;");
            return false;
        }
        query.finish();
    }
    qInfo("Migrations: migration '%s' has been completed", qPrintable(fileInfo.fileName()));
    if (!this->setSchemaVersion(fileInfo.fileName())) {
        qFatal("Migrations: Error updating the version table");
        query.exec("ROLLBACK;");
        return false;
    }
    query.exec("COMMIT;");
    return true;
}

/*!
 * \brief Migrations::extractDataFromVersionTable retrieves data from a version table
 * \return The current version of the database
 */
QString Migrations::extractDataFromVersionTable()
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT * FROM version_info")) {
        qFatal("Migrations: Error when extracting data from the version table: %s",
               qPrintable(query.lastError().text()));
        return "0";
    }
    if (!query.first()) {
        return "0";
    }
    QString version = query.value(0).toString();
    return version;
}

/*!
 * \brief Migrations::enshureVersionTableExist checks whether a table exists
 * \return A boolean value indicating the success or failure of the executed request
 */
bool Migrations::enshureVersionTableExist()
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='version_info';")) {
        qFatal("Migrations: Сhecking for the presence of a table failed: %s",
               qPrintable(query.lastError().text()));
        return false;
    }
    QString check = "0";
    if (query.first()) {
        check = query.value(0).toString();
    }
    if (check != "version_info") {
        if (!this->initializeMigrationTable()) {
            return false;
        }
    }
    return true;
}

/*!
 * \brief Migrations::performMigration executes the migration code
 * \return A boolean value indicating the success or failure of the database migration process being
 * performed
 */
bool Migrations::performMigration()
{
    if (!this->enshureVersionTableExist()) {
        qFatal("Migrations: The analysis of the migration table ended with an error. Migration "
               "completed ahead of schedule");
        return false;
    }
    QString version = extractDataFromVersionTable();
    QDir dir(":/migrations/");
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    for (QFileInfo file : list) {
        if (file.fileName() > version) {
            if (!this->openAndReadSqlFile(file)) {
                qFatal("Migrations: Error reading sql file. Migration completed ahead of "
                       "schedule.");
                return false;
            }
        }
    }
    return true;
}

void Migrations::setDb(QSqlDatabase db)
{
    m_db = db;
    emit dbChanged();
}

QSqlDatabase Migrations::db()
{
    return m_db;
}
