// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDesktopServices>
#include <QUrl>

#include "fileeditorcontroller.h"
#include "operations/fileinfooperation.h"
#include "operations/downloadfileoperation.h"

FileEditorController::FileEditorController(QObject *parent) : QObject(parent)
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this,
            &FileEditorController::fileChangedHandler);
    connect(this, &FileEditorController::fileContentChanged, this,
            &FileEditorController::fileChanged);
}

FileEditorController::~FileEditorController()
{
    emit abortOperations();
    closeFile();
}

/*!
 * \brief Creates and opens temporary local copy of a remote file.
 * \param path Path to the file on the cloud.
 */
void FileEditorController::openFile(const QString &path)
{
    closeFile();
    m_edited = false;
    getFileInfo(path, [=](bool status, QString networkError, const FileOrFolderElement fileinfo) {
        if (status) {
            downloadFile(path, [=](const bool success) {
                if (success) {
                    m_cacheTimestamp = fileinfo.timestamp;
                    emit cacheTimestampChanged();
                    QFileInfo file = QFileInfo(m_cache.localPath(path));
                    open(file);
                }
            });
        } else if (!networkError.isNull()) {
            emit errorOccured(networkError);
        }
    });
}

/*!
 * \brief Closes and deletes temporary local copy of a remote file.
 */
void FileEditorController::closeFile()
{
    if (m_file.exists()) {
        m_cache.deleteFromCache(m_file.absoluteFilePath());
        close(m_file);
    }
}

/*!
 * \brief Requests modification date of a remote file
 * and compares it to cache time of its local copy to determine
 * if a remote file has been modified.
 * \param path Path to the file on the cloud.
 */
void FileEditorController::checkForConflict(const QString &path)
{
    getFileInfo(path, [=](bool status, QString networkError, const FileOrFolderElement fileinfo) {
        if (status) {
            if (fileinfo.timestamp != m_cacheTimestamp) {
                emit conflictCheckDone(
                        true,
                        QString("File has been modified on the remote storage at %1.")
                                .arg(fileinfo.timestamp.toString("yyyy.MM.dd hh:mm:ss")));
            } else {
                emit conflictCheckDone(false, {});
            }
        } else if (networkError.isNull()) {
            emit conflictCheckDone(true, QString("File has been deleted from the remote storage."));
        } else {
            emit errorOccured(networkError);
        }
    });
}

/*!
 * \brief Saves copy of a cached file to a persistent directory.
 * \param dir Path to writable directory.
 * \return True if file was successfully saved.
 */
bool FileEditorController::saveTo(const QString &dir)
{
    return QFile::copy(m_file.absoluteFilePath(), dir + "/" + m_file.fileName());
}

/*!
 * \brief File downloading progress.
 * \return value in the range from 0.0 to 1.0
 */
float FileEditorController::downloadProgress()
{
    return m_downloadProgress;
}

/*!
 * \brief Is local copy was modified.
 * \return
 */
bool FileEditorController::isEdited()
{
    return m_edited;
}

/*!
 * \brief Timestamp of a remote file that was cached.
 * \return
 */
QDateTime FileEditorController::cacheTimestamp()
{
    return m_cacheTimestamp;
}

/*!
 * \brief Last modification time of a local copy.
 * \return
 */
QDateTime FileEditorController::lastModified()
{
    return m_file.lastModified();
}

/*!
 * \brief Path to the local copy.
 * \return
 */
QString FileEditorController::filePath()
{
    return m_file.absoluteFilePath();
}

/*!
 * \brief Open cached file in a system application and start tracking its changes.
 * \param file Cached file.
 */
void FileEditorController::open(QFileInfo file)
{
    if (m_file != file)
        close(file);
    m_file = file;
    emit fileChanged();
    m_watcher.addPath(file.absoluteFilePath());
    QDesktopServices::openUrl(QUrl("file://" + file.absoluteFilePath()));
}

/*!
 * \brief Stop watching cached file.
 * \param file Cached file
 */
void FileEditorController::close(QFileInfo file)
{
    m_file = {};
    emit fileChanged();
    m_watcher.removePath(file.absoluteFilePath());
}

/*!
 * \brief Update file download progress value.
 * \param percent Download completion value in the range from 0.0 to 1.0.
 */
void FileEditorController::setDownloadProgress(float percent)
{
    if (m_downloadProgress != percent) {
        m_downloadProgress = percent;
        emit downloadProgressChanged(percent);
    }
}

/*!
 * \brief Handles changes in the cached file content.
 * \param filename Absoluti path to a modified file.
 */
void FileEditorController::fileChangedHandler(QString filename)
{
    m_edited = true;
    m_file.refresh();
    emit fileContentChanged(filename);
}

/*!
 * \brief Run asynchronous download file operation and handle its result.
 * \param path Remote file path.
 * \param handler Handler for the operation result.
 */
template<class T>
void FileEditorController::downloadFile(const QString &path, T handler)
{
    m_downloadProgress = 0;
    DownloadFileOperation *op =
            new DownloadFileOperation(m_settings, path, m_cache.localPath(path));
    connect(this, &FileEditorController::abortOperations, op, &DownloadFileOperation::abort);
    connect(op, &DownloadFileOperation::progressChanged, this,
            &FileEditorController::setDownloadProgress);
    connect(op, &DownloadFileOperation::operationFinished, this,
            [=](const bool success, const QString) {
                handler(success);
                op->deleteLater();
            });
    op->run();
}

/*!
 * \brief Run asynchronous file info operation and handle its result.
 * \param path Remote file path.
 * \param handler Handler for the operation result.
 */
template<class T>
void FileEditorController::getFileInfo(const QString &path, T handler)
{
    FileInfoOperation *op = new FileInfoOperation(m_settings, path);
    connect(op, &FileInfoOperation::operationFinished, this,
            [=](bool status, QString networkError, const FileOrFolderElement fileinfo) {
                handler(status, networkError, fileinfo);
                op->deleteLater();
            });
    op->run();
}
