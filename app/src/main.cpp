// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QScopedPointer>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include <auroraapp.h>

#include "registertypes.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.auroraos"));
    application->setApplicationName(QStringLiteral("AuroraOsWebDavGuiClient"));

    CoreInit::registerTypes();
    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->rootContext()->setContextProperty("clientId", CLIENT_ID);
    view->rootContext()->setContextProperty("clientSecret", CLIENT_SECRET);
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/AuroraOsWebDavGuiClient.qml")));
    view->show();

    return application->exec();
}
