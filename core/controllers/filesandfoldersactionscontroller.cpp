// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "../models/fileandfolderlistmodel.h"
#include "../operations/createfolderoperation.h"
#include "../operations/downloadfileoperation.h"
#include "../operations/downloadfolderoperation.h"
#include "../operations/uploadfileoperation.h"
#include "../operations/uploadfolderoperation.h"
#include "../operations/renamefileordirectoryoperation.h"
#include "../operations/movefileordirectoryoperation.h"
#include "../operations/movefiletodeviceoperation.h"
#include "../operations/movefoldertodeviceoperation.h"
#include "../types/politicsflags.h"

#include "filesandfoldersactionscontroller.h"

/*!
 * \brief Constructor initializes the class fields.
 * \param parent QObject parent instance.
 */
FilesAndFoldersActionsController::FilesAndFoldersActionsController(QObject *parent)
    : ConnectionController(parent)
{
    m_entitiesModel = new FileAndFolderListModel(this);
}

/*!
 * \brief Checks for name conflict for file or folder.
 * \param documentName Name of the file (empty if it is folder).
 * \return True if there is a conflict.
 */
bool FilesAndFoldersActionsController::isConflict(QString documentName)
{
    if (m_entitiesModel->isPathExist(m_path + documentName))
        return true;
    else
        return false;
}

/*!
 * \brief FilesAndFoldersActionsController::getFilePath returns the path of the downloaded file
 * \param documentName Name of the document.
 * \param devicePath Path on the device to download in.
 * \param newName New name of the document.
 * \return
 */
QString FilesAndFoldersActionsController::getFilePath(QString documentName, QString devicePath,
                                                      QString newName)
{
    QString filePath = devicePath + "/";
    if (newName.isEmpty()) {
        filePath.append(documentName);
    } else
        filePath.append(newName);
    return filePath;
}

/*!
 * \brief Getter for entities model.
 * \return FileAndFolderListModel entitiesModel.
 */
FileAndFolderListModel *FilesAndFoldersActionsController::entitiesModel()
{
    return m_entitiesModel;
}

/*!
 * \brief Getter for notification header
 * \return errorMessage
 */
QString FilesAndFoldersActionsController::errorMessage()
{
    return m_errorMessage;
}

/*!
 * \brief Getter for message title
 * \return msgTitle
 */
QString FilesAndFoldersActionsController::msgTitle()
{
    return m_msgTitle;
}

/*!
 * \brief Setter for entities model.
 * \param model new entitiesModel.
 */
void FilesAndFoldersActionsController::setEntitiesModel(FileAndFolderListModel *model)
{
    m_entitiesModel = model;
}

/*!
 * \brief Initializes the CreateFolderOperation operation and sets up the signal-slot connection.
 */
void FilesAndFoldersActionsController::initCreateFolderOperation()
{
    auto createFolderOperation = new CreateFolderOperation(m_settings, m_path);
    connect(createFolderOperation, &CreateFolderOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleCreatingFolderOperation, Qt::UniqueConnection);
    setConnectionInProgress(true);
    createFolderOperation->run();
}

/*!
 * \brief Handles response from CreateFolderOperation.
 * \param success True if operation was successful.
 * \param errorString Error string.
 */
void FilesAndFoldersActionsController::handleCreatingFolderOperation(const bool success,
                                                                     const QString errorString)
{
    setConnectionInProgress(false);

    if (success) {
        m_msgTitle = QObject::tr("Folder %1 created successfully");
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Creation of folder %1 failed");
        m_errorMessage = errorString;
        emit operationError();
    }
}

/*!
 * \brief Initializes the UploadFileOperation operation and sets up the signal-slot connection.
 * \param documentPath Path to document on device.
 * \param documentName Name of the document.
 * \param actionFlag Flag to determine an action (overwrite or rename).
 */
void FilesAndFoldersActionsController::initUploadFileOperation(QString documentPath,
                                                               QString documentName, int actionFlag)
{
    if (actionFlag != PoliticsFlags::Overwrite && isConflict(documentName)) {
        emit conflictOccured();
        return;
    }

    auto uploadFileOperation =
            new UploadFileOperation(m_settings, m_path, documentPath, documentName);
    connect(uploadFileOperation, &UploadFileOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleUploadingFileOperation, Qt::UniqueConnection);
    setConnectionInProgress(true);
    uploadFileOperation->run();
}

/*!
 * \brief Handles response from UploadFileOperation.
 * \param success True if operation was successful.
 * \param errorString Error string.
 */
void FilesAndFoldersActionsController::handleUploadingFileOperation(const bool success,
                                                                    const QString errorString)
{
    setConnectionInProgress(false);

    if (success) {
        m_msgTitle = QObject::tr("File %1 uploaded successfully");
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Uploading of file %1 failed");
        m_errorMessage = errorString;
        emit operationError();
    }
}

/*!
 * \brief Initializes the RenameFileOrDirectoryOperation operation and sets up
 * the signal-slot connections
 * \param folderPath the path to the folder where the item is located
 * \param name file or folder name
 * \param newName new file or folder name
 */
void FilesAndFoldersActionsController::renameFilesOrFolders(QString folderPath, QString name,
                                                            QString newName)
{
    auto operation = new RenameFileOrDirectoryOperation(m_settings, folderPath, name, newName);
    connect(operation, &RenameFileOrDirectoryOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleRenameFilesAndFoldersOperation,
            Qt::QueuedConnection);
    operation->run();
}

/*!
 * \brief FolderPageController::handleRenameFilesAndFoldersOperation
 * \param success True if operation was successful
 * \param nameElement file or directory name
 * \param errorMsg error message
 */
void FilesAndFoldersActionsController::handleRenameFilesAndFoldersOperation(
        const bool success, const QString nameElement, const QString errorMsg)
{
    if (success) {
        m_msgTitle = QObject::tr("%1 renamed successfully").arg(nameElement);
        emit operationSuccess();
    } else {
        m_errorMessage = QObject::tr("Error renaming %1: %2").arg(nameElement).arg(errorMsg);
        emit operationError();
    }
}

/*!
 * \brief Runs the UploadFolderOperation operation and sets up the signal-slot connection.
 * \param folderPath Path to folder on device.
 */
void FilesAndFoldersActionsController::runUploadFolderOperation(QString folderPath)
{
    auto uploadFolderOperation = new UploadFolderOperation(m_settings, m_path, folderPath);
    connect(uploadFolderOperation, &UploadFolderOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleUploadingFolderOperation,
            Qt::UniqueConnection);
    connect(uploadFolderOperation, &UploadFolderOperation::creatingFoldersFinished, this,
            &FilesAndFoldersActionsController::operationProgressChanged, Qt::UniqueConnection);
    setConnectionInProgress(true);
    uploadFolderOperation->run();
}

/*!
 * \brief Handles response from UploadFolderOperation.
 * \param success True if operation was successful.
 * \param errorString Error string.
 */
void FilesAndFoldersActionsController::handleUploadingFolderOperation(const bool success,
                                                                      const QString errorString)
{
    setConnectionInProgress(false);

    if (success) {
        m_msgTitle = QObject::tr("Folder %1 uploaded successfully");
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Uploading of folder %1 failed");
        m_errorMessage = errorString;
        emit operationError();
    }
}

/*!
 * \brief Runs the DownloadFileOperation operation and sets up the signal-slot connection.
 * \param documentName Name of the document.
 * \param devicePath Path on the device to download in.
 */
void FilesAndFoldersActionsController::runDownloadFileOperation(QString documentName,
                                                                QString devicePath, bool overwrite,
                                                                QString newName)
{
    QString filePath = getFilePath(documentName, devicePath, newName);
    if (QFile::exists(filePath) && !overwrite) {
        emit conflictOccured();
        return;
    }
    auto downloadFileOperation = new DownloadFileOperation(m_settings, m_path, filePath);
    connect(downloadFileOperation, &DownloadFileOperation::operationFinished, this,
            [=](const bool success, const QString errorString) {
                handleDownloadingFileOperationResult(success, errorString);
                downloadFileOperation->deleteLater();
            });
    setConnectionInProgress(true);
    downloadFileOperation->run();
}

/*!
 * \brief Handles response from DownloadFileOperation.
 * \param success True if operation was successful.
 * \param errorString Error string.
 */
void FilesAndFoldersActionsController::handleDownloadingFileOperationResult(
        const bool success, const QString errorString)
{
    setConnectionInProgress(false);

    if (success) {
        m_msgTitle = QObject::tr("File %1 downloaded successfully");
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Downloading of file %1 failed");
        m_errorMessage = errorString;
        emit operationError();
    }
}

/*!
 * \brief Runs the DownloadFileOperation operation and sets up the signal-slot connection.
 * \param folderName Name of the folder.
 * \param devicePath Path on the device to download in.
 */
void FilesAndFoldersActionsController::runDownloadFolderOperation(QString documentName,
                                                                  QString devicePath,
                                                                  bool overwrite, QString newName)
{
    QString filePath = getFilePath(documentName, devicePath, newName);
    if (QDir(filePath).exists() && !overwrite) {
        emit conflictOccured();
        return;
    }
    auto downloadFolderOperation =
            new DownloadFolderOperation(m_settings, m_path, filePath, overwrite);
    connect(downloadFolderOperation, &DownloadFolderOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleDownloadingFolderOperation,
            Qt::UniqueConnection);
    connect(downloadFolderOperation, &DownloadFolderOperation::creatingFoldersFinished, this,
            &FilesAndFoldersActionsController::operationProgressChanged, Qt::UniqueConnection);
    setConnectionInProgress(true);
    downloadFolderOperation->run();
}

/*!
 * \brief Handles response from DownloadFolderOperation.
 * \param success True if operation was successful.
 * \param errorString Error string.
 */
void FilesAndFoldersActionsController::handleDownloadingFolderOperation(const bool success,
                                                                        const QString errorString)
{
    setConnectionInProgress(false);

    if (success) {
        m_msgTitle = QObject::tr("Folder %1 downloaded successfully");
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Downloading of folder %1 failed");
        m_errorMessage = errorString;
        emit operationError();
    }
}

/*!
 * \brief Runs the MoveFileOrDirectoryOperation operation and sets up the signal-slot connection
 * \param element moving element
 * \param folderNewPath The path to which the element is being moved
 * \param overwrite Permission to overwrite
 * \param newName New name of the element being moved
 */
void FilesAndFoldersActionsController::moveFilesOrFolders(FileOrFolderElement element,
                                                          QString folderNewPath, bool overwrite,
                                                          QString newName)
{
    QString nameNewPath = element.name;
    if (!newName.isEmpty())
        nameNewPath = newName;
    if (!element.is_file)
        nameNewPath += "/";
    if (isConflict(nameNewPath) && !overwrite) {
        emit conflictOccured();
        return;
    }
    auto operation = new MoveFileOrDirectoryOperation(m_settings, element.path,
                                                      folderNewPath + nameNewPath, overwrite);
    connect(operation, &MoveFileOrDirectoryOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleMoveFilesAndFoldersOperation,
            Qt::QueuedConnection);
    operation->run();
}

/*!
 * \brief Handles response from MoveFileOrDirectoryOperation
 * \param success True if operation was successful
 * \param pathElement Element's path
 * \param errorMsg Error string
 */
void FilesAndFoldersActionsController::handleMoveFilesAndFoldersOperation(const bool success,
                                                                          const QString pathElement,
                                                                          const QString errorMsg)
{
    if (success) {
        m_msgTitle = QObject::tr("%1 moved successfully").arg(nameElement(pathElement));
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Error moving %1").arg(nameElement(pathElement));
        m_errorMessage = errorMsg;
        emit operationError();
    }
}

/*!
 * \brief Runs the MoveFileToDeviceOperation operation and sets up the signal-slot connection.
 * \param documentName Name of the document.
 * \param devicePath Path on the device to download in.
 */
void FilesAndFoldersActionsController::runMoveFileToDeviceOperation(QString documentName,
                                                                    QString devicePath,
                                                                    bool overwrite, QString newName)
{
    QString filePath = getFilePath(documentName, devicePath, newName);
    if (QFile::exists(filePath) && !overwrite) {
        emit conflictOccured();
        return;
    }
    auto moveFileToDeviceOperation = new MoveFileToDeviceOperation(m_settings, m_path, filePath);
    connect(moveFileToDeviceOperation, &MoveFileToDeviceOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleMoveFileToDeviceOperation,
            Qt::UniqueConnection);
    connect(moveFileToDeviceOperation, &MoveFileToDeviceOperation::downloadFinished, this,
            &FilesAndFoldersActionsController::operationProgressChanged, Qt::UniqueConnection);
    setConnectionInProgress(true);
    moveFileToDeviceOperation->run();
}

/*!
 * \brief Handles response from MoveFileToDeviceOperation
 * \param success True if operation was successful
 * \param errorMsg Error string
 */
void FilesAndFoldersActionsController::handleMoveFileToDeviceOperation(const bool success,
                                                                       const QString errorMsg)
{
    if (success) {
        m_msgTitle = QObject::tr("File %1 moved to device successfully");
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Error moving file %1 to device");
        m_errorMessage = errorMsg;
        emit operationError();
    }
}

/*!
 * \brief Runs the MoveFolderToDeviceOperation operation and sets up the signal-slot connection.
 * \param devicePath Path on the device to download in.
 */
void FilesAndFoldersActionsController::runMoveFolderToDeviceOperation(QString documentName,
                                                                      QString devicePath,
                                                                      bool overwrite,
                                                                      QString newName)
{
    QString filePath = getFilePath(documentName, devicePath, newName);
    if (QDir(filePath).exists() && !overwrite) {
        emit conflictOccured();
        return;
    }
    auto moveFolderToDeviceOperation =
            new MoveFolderToDeviceOperation(m_settings, m_path, filePath, overwrite);
    connect(moveFolderToDeviceOperation, &MoveFolderToDeviceOperation::operationFinished, this,
            &FilesAndFoldersActionsController::handleMoveFolderToDeviceOperation,
            Qt::UniqueConnection);
    connect(moveFolderToDeviceOperation, &MoveFolderToDeviceOperation::downloadFinished, this,
            &FilesAndFoldersActionsController::operationProgressChanged, Qt::UniqueConnection);
    setConnectionInProgress(true);
    moveFolderToDeviceOperation->run();
}

/*!
 * \brief Handles response from MoveFolderToDeviceOperation
 * \param success True if operation was successful
 * \param errorString Error string
 */
void FilesAndFoldersActionsController::handleMoveFolderToDeviceOperation(const bool success,
                                                                         const QString errorString)
{
    if (success) {
        m_msgTitle = QObject::tr("Folder %1 moved to device successfully");
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Error moving folder %1 to device");
        m_errorMessage = errorString;
        emit operationError();
    }
}
