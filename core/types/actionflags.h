// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef ACTIONFLAGS_H
#define ACTIONFLAGS_H

// Qt headers
#include <QObject>

/*!
 * \brief The ActionFlags class provides flags, that describes different actions.
 */
class ActionFlags
{
    Q_GADGET

public:
    enum Flags : qint8 {
        Download,
        MoveToDevice,
        Upload,
    };
    Q_ENUM(Flags);

    static QString actionFlagToString(ActionFlags::Flags flag)
    {
        switch (flag) {
        case Download:
            return QStringLiteral("Download");
        case MoveToDevice:
            return QStringLiteral("Move to device");
        case Upload:
            return QStringLiteral("Upload");
        }

        return {};
    }
};

#endif // ACTIONFLAGS_H
