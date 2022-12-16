// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "accountselectioncontroller.h"
#include "../models/remoteservicemodel.h"

/*!
 * \brief AccountSelectionController::AccountSelectionController constructor
 * \param parent
 */
AccountSelectionController::AccountSelectionController(QObject *parent)
    : ConnectionController(parent)
{
    m_remoteServiceModel = new RemoteServiceModel(this);
}

/*!
 * \brief AccountSelectionController::updateModelData updates the model
 * to display a list of accounts
 */
void AccountSelectionController::updateModelData()
{
    QList<UserElement> listItems = m_dao->extractAccountInformation();
    m_remoteServiceModel->setElementsForModel(listItems);
}

/*!
 * \brief AccountSelectionController::deleteAccount removes an account from the model
 * \param id id of the account being deleted
 */
void AccountSelectionController::deleteAccount(int id)
{
    connect(m_dao, &DataAccessObject::deleteUsersChanged, this,
            &AccountSelectionController::handleDeleteUser, Qt::UniqueConnection);
    m_remoteServiceModel->setBusy(id, true);
    m_dao->deleteUsers(id);
}

/*!
 * \brief AccountSelectionController::handleDeleteUser
 * \param success True if operation was successful
 * \param id The id to the account to be deleted
 * \param errorMsg error message
 */
void AccountSelectionController::handleDeleteUser(bool success, int id, QString errorMsg)
{
    m_remoteServiceModel->setBusy(id, false);
    QString userName = m_remoteServiceModel->userNameById(id);

    if (success) {
        m_remoteServiceModel->deleteById(id);
        m_msgTitle = QObject::tr("%1 deleted successfully").arg(userName);
        emit operationSuccess();
    } else {
        m_msgTitle = QObject::tr("Error deleting %1").arg(userName);
        m_errorMessage = errorMsg;
        emit operationError();
    }
}

/*!
 * \brief Getter for entities model.
 * \return RemoteServiceModel remoteServiceModel.
 */
RemoteServiceModel *AccountSelectionController::remoteServiceModel()
{
    return m_remoteServiceModel;
}

/*!
 * \brief Getter for notification header
 * \return errorMessage
 */
QString AccountSelectionController::errorMessage()
{
    return m_errorMessage;
}

/*!
 * \brief Getter for entities model
 * \return msgTitle
 */
QString AccountSelectionController::msgTitle()
{
    return m_msgTitle;
}
