// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef USERELEMENT_H
#define USERELEMENT_H

#include <QObject>
#include <QDateTime>

#include "types/token.h"

/*!
 * \brief The UserElement struct universal structure for an account
 */
struct UserElement
{
    // Constructor
    UserElement(QString portalAddress = {}, QString userName = {}, QString password = {},
                QString service = {}, bool busy = false, Token token = {})
        : portalAddress(portalAddress),
          userName(userName),
          password(password),
          service(service),
          busy(busy),
          token(token)
    {
    }

    int id;
    QString portalAddress;
    QString userName;
    QString password;
    QString service;
    bool busy;
    Token token;
};

Q_DECLARE_METATYPE(UserElement);

#endif // USERELEMENT_H
