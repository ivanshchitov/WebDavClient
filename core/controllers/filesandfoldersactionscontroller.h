// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FILESANDFOLDERSACTIONSCONTROLLER_H
#define FILESANDFOLDERSACTIONSCONTROLLER_H

#include <QObject>
#include <qwebdav.h>

#include "connectioncontroller.h"
#include "types/fileorfolderelement.h"
#include "utils/localmacros.h"

class FileAndFolderListModel;
class LoadFilesAndFoldersOperation;
class CreateFolderOperation;

class FilesAndFoldersActionsController : public ConnectionController
{
    Q_OBJECT
    Q_PROPERTY(FileAndFolderListModel *entitiesModel READ entitiesModel WRITE setEntitiesModel
                       NOTIFY entitiesModelChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage);
    Q_PROPERTY(QString msgTitle READ msgTitle);
    MAKE_INTERFACE_PROPERTY(Settings *, settings, Settings);
    MAKE_INTERFACE_PROPERTY(QString, path, Path);

public:
    explicit FilesAndFoldersActionsController(QObject *parent = nullptr);

    FileAndFolderListModel *entitiesModel();
    QString errorMessage();
    QString msgTitle();

    Q_INVOKABLE void setEntitiesModel(FileAndFolderListModel *model);
    Q_INVOKABLE void initCreateFolderOperation();
    Q_INVOKABLE void initUploadFileOperation(QString documentPath, QString documentName,
                                             int actionFlag);
    Q_INVOKABLE void runUploadFolderOperation(QString folderPath);
    Q_INVOKABLE void renameFilesOrFolders(QString folderPath, QString name, QString newName);
    Q_INVOKABLE void runDownloadFileOperation(QString documentName, QString devicePath,
                                              bool overwrite, QString newName);
    Q_INVOKABLE void runDownloadFolderOperation(QString documentName, QString devicePath,
                                                bool overwrite, QString newName);
    Q_INVOKABLE void moveFilesOrFolders(FileOrFolderElement element, QString newPath,
                                        bool overwrite, QString newName);
    Q_INVOKABLE void runMoveFileToDeviceOperation(QString documentName, QString devicePath,
                                                  bool overwrite, QString newName);
    Q_INVOKABLE void runMoveFolderToDeviceOperation(QString documentName, QString devicePath,
                                                    bool overwrite, QString newName);

private slots:
    void handleCreatingFolderOperation(const bool success, const QString errorString = nullptr);
    void handleDownloadingFileOperationResult(const bool success,
                                              const QString errorString = nullptr);
    void handleDownloadingFolderOperation(const bool success, const QString errorString = nullptr);
    void handleUploadingFileOperation(const bool success, const QString errorString = nullptr);
    void handleUploadingFolderOperation(const bool success, const QString errorString = nullptr);
    void handleRenameFilesAndFoldersOperation(const bool success, const QString name,
                                              const QString errorMsg);
    void handleMoveFilesAndFoldersOperation(const bool success, const QString pathElement,
                                            const QString errorMsg);
    void handleMoveFileToDeviceOperation(const bool success, const QString errorMsg);
    void handleMoveFolderToDeviceOperation(const bool success, const QString errorString = nullptr);

private:
    bool isConflict(QString documentName = nullptr);
    QString getFilePath(QString documentName, QString devicePath, QString newName);

private:
    FileAndFolderListModel *m_entitiesModel;
    QString m_errorMessage;
    QString m_msgTitle;

signals:
    void entitiesModelChanged();
    void operationError();
    void operationSuccess();
    void conflictOccured();
    void operationProgressChanged();
};

#endif // FILESANDFOLDERSACTIONSCONTROLLER_H
