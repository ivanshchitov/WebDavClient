# SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
# SPDX-License-Identifier: BSD-3-Clause

TARGET = ru.auroraos.AuroraOsWebDavGuiClient
TEMPLATE = app
QT += sql
CONFIG += \
    auroraapp

PKGCONFIG += \

PROJECT_DIR = $$PWD/..
BUILD_DIR = $$OUT_PWD/..

isEmpty(CLIENT_ID) {
    error("CLIENT_ID is not set")
} else {
    DEFINES += \
        CLIENT_ID=\\\"$${CLIENT_ID}\\\"
}

isEmpty(CLIENT_SECRET) {
    error("CLIENT_SECRET is not set")
} else {
    DEFINES += \
        CLIENT_SECRET=\\\"$${CLIENT_SECRET}\\\"
}

include(../common.pri)

# core
INCLUDEPATH += $$PROJECT_DIR/core
LIBS += -L$$BUILD_DIR/core -lcore
PRE_TARGETDEPS += $$BUILD_DIR/core/libcore.a

# QtWebDAV
INCLUDEPATH += $$PROJECT_DIR/qwebdavlib/
LIBS += -L$$BUILD_DIR/qwebdavlib/ -lqwebdav

SOURCES += \
    src/main.cpp \

HEADERS += \

DISTFILES += \
    rpm/ru.auroraos.AuroraOsWebDavGuiClient.spec \
    LICENSE.BSD-3-CLAUSE.md \
    README.md \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.auroraos.AuroraOsWebDavGuiClient.ts \
    translations/ru.auroraos.AuroraOsWebDavGuiClient-ru.ts \

TRANSLATION_SOURCES += $${PWD}/../core

RESOURCES += \
    resources.qrc
