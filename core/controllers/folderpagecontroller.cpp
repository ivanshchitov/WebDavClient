// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "../models/fileandfolderlistmodel.h"
#include "../operations/loadfilesandfoldersoperation.h"
#include "../operations/deletefileordirectoryoperation.h"
#include "../operations/deletecommunigatedirectoryoperation.h"
#include "../operations/renamefileordirectoryoperation.h"
#include "../operations/downloadfileoperation.h"
#include "../operations/fileinfooperation.h"
#include "../types/politicsflags.h"
#include "folderpagecontroller.h"
#include "deletecontroller.h"

#include <QThread>

/*!
 * \brief Constructor initializes the class fields.
 * \param parent QObject parent instance.
 */
FolderPageController::FolderPageController(QObject *parent) : ConnectionController(parent)
{
    m_entitiesModel = new FileAndFolderListModel(this);
}

/*!
 * \brief Getter for entities model.
 * \return FileAndFolderListModel entitiesModel.
 */
FileAndFolderListModel *FolderPageController::entitiesModel()
{
    return m_entitiesModel;
}

/*!
 * \brief Getter for notification header
 * \return errorMessage
 */
QString FolderPageController::errorMessage()
{
    return m_errorMessage;
}

/*!
 * \brief Getter for message title
 * \return msgTitle
 */
QString FolderPageController::msgTitle()
{
    return m_msgTitle;
}

/*!
 * \brief FolderPageController::getCurrentItem returns data at the specified path
 * \param path
 * \return data
 */
FileOrFolderElement FolderPageController::getCurrentItem(QString path) const
{
    return m_entitiesModel->getElement(path);
}

/*!
 * \brief FolderPageController::clearModel clear the model
 */
void FolderPageController::clearModel()
{
    m_entitiesModel->clear();
}

/*!
 * \brief Initializes the LoadFilesAndFoldersOperation operation and sets up the signal-slot
 * connection.
 */
void FolderPageController::initLoadFilesAndFoldersOperation()
{
    auto loadFoldersAndFilesOperation = new LoadFilesAndFoldersOperation(m_settings, m_path);
    connect(loadFoldersAndFilesOperation, &LoadFilesAndFoldersOperation::operationFinished, this,
            &FolderPageController::handleLoadFilesAndFoldersOperation, Qt::QueuedConnection);
    setConnectionInProgress(true);
    loadFoldersAndFilesOperation->run();
}

/*!
 * \brief Handles response from LoadFilesAndFoldersOperation.
 * \param success True if operation was successful.
 * \param entities List with entities in current folder.
 */
void FolderPageController::handleLoadFilesAndFoldersOperation(
        const bool success, const QList<FileOrFolderElement> entities)
{
    setConnectionInProgress(false);
    if (success)
        m_entitiesModel->setElementsForModel(entities);
    emit filesAndFoldersLoadingFinished();
}

/*!
 * \brief FolderPageController::deleteFilesOrFolders starts the process of deleting a file or folder
 * \param pathElement Path to the item to be deleted
 * \param isFile true if the item being deleted is a file
 */
void FolderPageController::deleteFilesOrFolders(QString pathElement, bool isFile)
{
    m_entitiesModel->setBusy(pathElement, true);

    DeleteController *worker = new DeleteController(m_settings, pathElement, isFile);
    QThread *thread = new QThread();
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &DeleteController::run);
    connect(worker, &DeleteController::finished, this,
            &FolderPageController::handleDeleteFilesAndFoldersOperation);
    connect(worker, &DeleteController::finished, thread, &QThread::quit);
    connect(worker, &DeleteController::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
}

/*!
 * \brief FolderPageController::handleDeleteFilesAndFoldersOperation
 * \param success True if operation was successful
 * \param pathElement The path to the item to be deleted
 * \param errorMsg error message
 */
void FolderPageController::handleDeleteFilesAndFoldersOperation(const bool success,
                                                                const QString pathElement,
                                                                const QString errorMsg)
{
    m_entitiesModel->setBusy(pathElement, false);

    if (success) {
        m_entitiesModel->deleteByPath(pathElement);
        m_msgTitle = QObject::tr("%1 deleted successfully").arg(nameElement(pathElement));
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Error deleting %1").arg(nameElement(pathElement));
        m_errorMessage = errorMsg;
        emit operationError();
    }
}
