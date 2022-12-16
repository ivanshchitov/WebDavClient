// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QtQuick>

#include <qwebdav.h>
#include <qwebdavdirparser.h>
#include <qwebdavitem.h>

#include "controllers/folderpagecontroller.h"
#include "controllers/filesandfoldersactionscontroller.h"
#include "controllers/connectioncontroller.h"
#include "controllers/accountselectioncontroller.h"
#include "controllers/renamecontroller.h"
#include "controllers/fileeditorcontroller.h"
#include "controllers/foldersizequestiondialogcontroller.h"
#include "controllers/selectfolderdialogcontroller.h"
#include "controllers/oauthprovider.h"

#include "models/elementlistsortfiltermodel.h"
#include "models/fileandfolderlistmodel.h"
#include "models/remoteservicemodel.h"

#include "types/servicesflags.h"
#include "types/fileorfolderelement.h"
#include "types/politicsflags.h"
#include "types/actionflags.h"
#include "types/token.h"

#include "utils/urlhelper.h"

#include "registertypes.h"
#include "dataaccessobject.h"
#include "migrations.h"

#define REGISTER_URI "ru.auroraos.AuroraOsWebDavGuiClient"
#define UNCREATABLE_ERROR "Error: this type is not creatable in qml."

/*!
 * \brief This method registers cpp classes as qml objects.
 */
void CoreInit::registerTypes()
{
    qmlRegisterType<FolderPageController>(REGISTER_URI, 1, 0, "FolderPageController");
    qmlRegisterType<FilesAndFoldersActionsController>(REGISTER_URI, 1, 0,
                                                      "FilesAndFoldersActionsController");
    qmlRegisterType<ConnectionController>(REGISTER_URI, 1, 0, "ConnectionController");
    qmlRegisterType<AccountSelectionController>(REGISTER_URI, 1, 0, "AccountSelectionController");
    qmlRegisterType<RenameController>(REGISTER_URI, 1, 0, "RenameController");
    qmlRegisterType<FileEditorController>(REGISTER_URI, 1, 0, "FileEditorController");
    qmlRegisterType<FolderSizeQuestionDialogController>(REGISTER_URI, 1, 0,
                                                        "FolderSizeQuestionDialogController");
    qmlRegisterType<SelectFolderDialogController>(REGISTER_URI, 1, 0,
                                                  "SelectFolderDialogController");
    qmlRegisterType<QWebdav>(REGISTER_URI, 1, 0, "QWebdav");

    qmlRegisterType<Settings>(REGISTER_URI, 1, 0, "Settings");
    qmlRegisterType<DataAccessObject>(REGISTER_URI, 1, 0, "DataAccessObject");
    qmlRegisterType<Migrations>(REGISTER_URI, 1, 0, "Migrations");

    qmlRegisterType<ElementListSortFilterModel>(REGISTER_URI, 1, 0, "ElementListSortFilterModel");
    qmlRegisterType<FileAndFolderListModel>(REGISTER_URI, 1, 0, "FileAndFolderListModel");
    qmlRegisterType<RemoteServiceModel>(REGISTER_URI, 1, 0, "RemoteServiceModel");
    qmlRegisterType<OAuthProvider>(REGISTER_URI, 1, 0, "OAuthProvider");

    qRegisterMetaType<QList<UserElement>>();
    qRegisterMetaType<FileOrFolderElement>();
    qRegisterMetaType<QList<FileOrFolderElement>>();

    qmlRegisterType<Settings>(REGISTER_URI, 1, 0, "Settings");
    qmlRegisterType<ElementListSortFilterModel>(REGISTER_URI, 1, 0, "ElementListSortFilterModel");
    qmlRegisterType<FileAndFolderListModel>(REGISTER_URI, 1, 0, "FileAndFolderListModel");
    qmlRegisterUncreatableType<ServicesFlags>(REGISTER_URI, 1, 0, "ServicesFlags",
                                              UNCREATABLE_ERROR);
    qmlRegisterUncreatableType<PoliticsFlags>(REGISTER_URI, 1, 0, "PoliticsFlags",
                                              UNCREATABLE_ERROR);
    qmlRegisterUncreatableType<ActionFlags>(REGISTER_URI, 1, 0, "ActionFlags", UNCREATABLE_ERROR);
    qmlRegisterUncreatableType<Token>(REGISTER_URI, 1, 0, "Token", UNCREATABLE_ERROR);

    qRegisterMetaType<QWebdav *>();

    UrlHelper::registerSingleton(REGISTER_URI);
}
