// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SORTINGOPTIONS_H
#define SORTINGOPTIONS_H

#include <QObject>

/*!
 * \brief The SortingOptions class provides values
 * that characterize the sorting of files and directories
 */
class SortingOptions
{
    Q_GADGET

public:
    enum State : quint8 {
        None,
        Date,
        Name,
        Type,
        Size,
        Author,
    };
    Q_ENUM(State)
};

#endif // SORTINGOPTIONS_H
