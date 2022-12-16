# SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
# SPDX-License-Identifier: BSD-3-Clause

TEMPLATE = lib
TARGET = core
QT += sql
CONFIG += staticlib c++17

include(../common.pri)

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

# QtWebDAV
INCLUDEPATH += $$PROJECT_DIR/qwebdavlib/
LIBS += -L$$BUILD_DIR/qwebdavlib/ -lqwebdav

HEADERS = \
    controllers/deletecontroller.h \
    controllers/foldersizequestiondialogcontroller.h \
    controllers/oauthprovider.h \
    controllers/renamecontroller.h \
    controllers/fileeditorcontroller.h \
    controllers/selectfolderdialogcontroller.h \
    dataaccessobject.h \
    filecache.h \
    migrations.h \
    controllers/accountselectioncontroller.h \
    controllers/filesandfoldersactionscontroller.h \
    controllers/folderpagecontroller.h \
    controllers/connectioncontroller.h \
    models/elementlistsortfiltermodel.h \
    models/fileandfolderlistmodel.h \
    models/remoteservicemodel.h \
    operations/connectioncheckoperation.h \
    operations/createfolderoperation.h \
    operations/deletecommunigatedirectoryoperation.h \
    operations/deletefileordirectoryoperation.h \
    operations/downloadfileoperation.h \
    operations/fileinfooperation.h \
    operations/downloadfolderoperation.h \
    operations/foldersizecalculatingoperation.h \
    operations/getuserinfooperation.h \
    operations/loadfilesandfoldersoperation.h \
    operations/movefileordirectoryoperation.h \
    operations/movefiletodeviceoperation.h \
    operations/movefoldertodeviceoperation.h \
    operations/renamefileordirectoryoperation.h \
    operations/uploadfileoperation.h \
    operations/uploadfolderoperation.h \
    registertypes.h \
    settings.h \
    sortingoptions.h \
    types/actionflags.h \
    types/fileorfolderelement.h \
    types/politicsflags.h \
    types/servicesflags.h \
    types/token.h \
    types/userelement.h \
    utils/localmacros.h \
    utils/urlhelper.h \


SOURCES = \
    controllers/deletecontroller.cpp \
    controllers/foldersizequestiondialogcontroller.cpp \
    controllers/oauthprovider.cpp \
    controllers/renamecontroller.cpp \
    controllers/fileeditorcontroller.cpp \
    controllers/selectfolderdialogcontroller.cpp \
    dataaccessobject.cpp \
    filecache.cpp \
    migrations.cpp \
    controllers/accountselectioncontroller.cpp \
    controllers/filesandfoldersactionscontroller.cpp \
    controllers/folderpagecontroller.cpp \
    controllers/connectioncontroller.cpp \
    models/remoteservicemodel.cpp \
    operations/connectioncheckoperation.cpp \
    operations/deletecommunigatedirectoryoperation.cpp \
    operations/downloadfileoperation.cpp \
    operations/downloadfolderoperation.cpp \
    operations/foldersizecalculatingoperation.cpp \
    operations/movefileordirectoryoperation.cpp \
    operations/movefiletodeviceoperation.cpp \
    operations/movefoldertodeviceoperation.cpp \
    operations/uploadfolderoperation.cpp \
    operations/renamefileordirectoryoperation.cpp \
    operations/downloadfileoperation.cpp \
    operations/fileinfooperation.cpp \
    types/fileorfolderelement.cpp \
    types/token.cpp \
    models/elementlistsortfiltermodel.cpp \
    models/fileandfolderlistmodel.cpp \
    operations/createfolderoperation.cpp \
    operations/deletefileordirectoryoperation.cpp \
    operations/getuserinfooperation.cpp \
    operations/loadfilesandfoldersoperation.cpp \
    operations/uploadfileoperation.cpp \
    registertypes.cpp \
    settings.cpp \
    utils/urlhelper.cpp \

INCLUDEPATH += \
