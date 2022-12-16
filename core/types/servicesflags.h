// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SERVICESFLAGS_H
#define SERVICESFLAGS_H

// Qt headers
#include <QObject>

class ServicesFlags
{
    Q_GADGET

public:
    enum Flags : qint8 {
        NextCloud,
        YandexDisk,
        Communigate,
        WebDavServer,
    };
    Q_ENUM(Flags);

    static QString servicesFlagToString(ServicesFlags::Flags flag)
    {
        switch (flag) {
        case NextCloud:
            return QStringLiteral("NextCloud");
        case YandexDisk:
            return QStringLiteral("YandexDisk");
        case Communigate:
            return QStringLiteral("Communigate");
        case WebDavServer:
            return QStringLiteral("WebDavServer");
        }

        return {};
    }
};

#endif // SERVICESFLAGS_H
