// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "selectfolderdialogcontroller.h"
#include "operations/movefileordirectoryoperation.h"

/*!
 * \brief Constructor
 * \param parent QObject parent instance
 */
SelectFolderDialogController::SelectFolderDialogController(QObject *parent)
    : FolderPageController(parent)
{
}

/*!
 * \brief SelectFolderDialogController::abilityToReturn Sets the ability to go back down directories
 * \param path Directory path
 * \return True if it is possible to go down
 */
bool SelectFolderDialogController::abilityToReturn(QString path)
{
    return path != "/";
}

/*!
 * \brief SelectFolderDialogController::abilityToInteract Sets the ability to interact with an
 * element when selecting a folder to move
 * \param element File or folder element
 * \param path File or folder move path
 * \param isFile True if the file
 * \return True if there is a possibility of interaction
 */
bool SelectFolderDialogController::abilityToInteract(FileOrFolderElement element, QString path,
                                                     bool isFile)
{
    if (isFile || element.path == path)
        return false;
    return true;
}

/*!
 * \brief SelectFolderDialogController::returnParentPath Returns the path of the parent folder
 * \param path Folder path
 * \return path of the parent folder
 */
QString SelectFolderDialogController::returnParentPath(QString path)
{
    return path.left(path.lastIndexOf(QChar('/'), -2)) + "/";
}

/*!
 * \brief SelectFolderDialogController::returnParentName Returns the name of the parent folder
 * \param path Folder path
 * \return name of the parent folder
 */
QString SelectFolderDialogController::returnParentName(QString path)
{
    if (abilityToReturn(path))
        return nameElement(path);
    return QObject::tr("All files");
}
