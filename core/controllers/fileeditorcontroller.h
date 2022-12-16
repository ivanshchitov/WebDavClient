// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FILEEDITORCONTROLLER_H
#define FILEEDITORCONTROLLER_H

#include <QFileSystemWatcher>
#include <QDateTime>

#include "filecache.h"
#include "connectioncontroller.h"
#include "types/fileorfolderelement.h"
#include "utils/localmacros.h"

/*!
 * \brief Class that controlls remote file editing process
 */
class FileEditorController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float downloadProgress READ downloadProgress NOTIFY downloadProgressChanged)
    Q_PROPERTY(bool isEdited READ isEdited NOTIFY fileContentChanged)
    Q_PROPERTY(QDateTime cacheTimestamp READ cacheTimestamp NOTIFY cacheTimestampChanged)
    Q_PROPERTY(QDateTime lastModified READ lastModified NOTIFY fileChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY fileChanged)
    MAKE_INTERFACE_PROPERTY(Settings *, settings, Settings);

public:
    explicit FileEditorController(QObject *parent = nullptr);
    ~FileEditorController();
    Q_INVOKABLE void openFile(const QString &path);
    Q_INVOKABLE void closeFile();
    Q_INVOKABLE void checkForConflict(const QString &path);
    Q_INVOKABLE bool saveTo(const QString &dir);
    float downloadProgress();
    bool isEdited();
    QDateTime cacheTimestamp();
    QDateTime lastModified();
    QString filePath();

signals:
    void downloadProgressChanged(float percent);
    void fileContentChanged(QString file);
    void fileChanged();
    void cacheTimestampChanged();
    void abortOperations();
    void conflictCheckDone(bool status, QString reason);
    void errorOccured(QString errorMessage);

private slots:
    void fileChangedHandler(QString filename);

private:
    QFileSystemWatcher m_watcher;
    FileCache m_cache;
    bool m_edited = false;
    float m_downloadProgress;

    QDateTime m_cacheTimestamp;
    QDateTime m_editTimestamp;
    QFileInfo m_file;

    void open(QFileInfo file);
    void close(QFileInfo file);
    void setDownloadProgress(float percent);

    template<class T>
    void downloadFile(const QString &path, T handler);
    template<class T>
    void getFileInfo(const QString &path, T handler);
};

#endif // FILEEDITORCONTROLLER_H
