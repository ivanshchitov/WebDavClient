// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "foldersizequestiondialogcontroller.h"
#include "../operations/foldersizecalculatingoperation.h"

/*!
 * \brief Constructor initializes the class fields.
 * \param parent QObject parent instance.
 */
FolderSizeQuestionDialogController::FolderSizeQuestionDialogController(QObject *parent)
    : ConnectionController(parent)
{
}

/*!
 * \brief Getter for notification header.
 * \return errorMessage
 */
QString FolderSizeQuestionDialogController::errorMessage()
{
    return m_errorMessage;
}

/*!
 * \brief Getter for message title.
 * \return msgTitle
 */
QString FolderSizeQuestionDialogController::msgTitle()
{
    return m_msgTitle;
}

/*!
 * \brief Getter for folder size.
 * \return folderSize
 */
quint64 FolderSizeQuestionDialogController::folderSize()
{
    return m_folderSize;
}

/*!
 * \brief Initializes the FolderSizeCalculatingOperation operation and sets up the signal-slot
 * connection.
 */
void FolderSizeQuestionDialogController::runFolderSizeCalculatingOperation(QString folderName)
{
    auto folderSizeCalculatingOperation =
            new FolderSizeCalculatingOperation(m_settings, m_path, folderName);
    connect(folderSizeCalculatingOperation, &FolderSizeCalculatingOperation::operationFinished,
            this, &FolderSizeQuestionDialogController::handleFolderSizeCalculatingOperation,
            Qt::UniqueConnection);
    setConnectionInProgress(true);
    folderSizeCalculatingOperation->run();
}

/*!
 * \brief FolderSizeQuestionDialogController::handleFolderSizeCalculatingOperation
 * \param success True if operation was successful
 * \param errorString Error string.
 * \param folderSize Current folder size.
 */
void FolderSizeQuestionDialogController::handleFolderSizeCalculatingOperation(
        const bool success, const QString errorString, quint64 folderSize)
{
    setConnectionInProgress(false);

    if (success) {
        m_folderSize = folderSize;
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Calculation of folder size failed");
        m_errorMessage = errorString;
        emit operationError();
    }
}
