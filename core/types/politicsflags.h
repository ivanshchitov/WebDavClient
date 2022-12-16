// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef POLITICSFLAGS_H
#define POLITICSFLAGS_H

// Qt headers
#include <QObject>

/*!
 * \brief The PoliticsFlags class Provides flags, that describes a
 * actions with files when operation in process.
 */
class PoliticsFlags
{
    Q_GADGET

public:
    enum Flags : qint8 {
        Overwrite,
        Rename,
    };
    Q_ENUM(Flags);

    static QString copyFlagToString(PoliticsFlags::Flags flag)
    {
        switch (flag) {
        case Overwrite:
            return QStringLiteral("Overwrite");
        case Rename:
            return QStringLiteral("Rename");
        }

        return {};
    }
};

#endif // POLITICSFLAGS_H
