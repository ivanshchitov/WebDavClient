// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef FOLDERPAGECONTROLLER_H
#define FOLDERPAGECONTROLLER_H

#include <QObject>
#include <qwebdav.h>

#include "connectioncontroller.h"
#include "types/fileorfolderelement.h"
#include "utils/localmacros.h"

class FileAndFolderListModel;
class LoadFilesAndFoldersOperation;
class CreateFolderOperation;

/*!
 * \brief The FolderPageController class is a controller for the folder page.
 */
class FolderPageController : public ConnectionController
{
    Q_OBJECT
    Q_PROPERTY(FileAndFolderListModel *entitiesModel READ entitiesModel NOTIFY entitiesModelChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage);
    Q_PROPERTY(QString msgTitle READ msgTitle);
    MAKE_INTERFACE_PROPERTY(QString, path, Path);

public:
    explicit FolderPageController(QObject *parent = nullptr);

    FileAndFolderListModel *entitiesModel();
    QString errorMessage();
    QString msgTitle();

    Q_INVOKABLE void deleteFilesOrFolders(QString pathElement, bool isFile);
    Q_INVOKABLE void initLoadFilesAndFoldersOperation();
    Q_INVOKABLE FileOrFolderElement getCurrentItem(QString path) const;
    Q_INVOKABLE void clearModel();

private slots:
    void handleLoadFilesAndFoldersOperation(const bool success,
                                            const QList<FileOrFolderElement> entities);
    void handleDeleteFilesAndFoldersOperation(const bool success, const QString pathElement,
                                              const QString errorMsg);

private:
    bool checkConflict(QString documentName = nullptr);

private:
    FileAndFolderListModel *m_entitiesModel;
    QString m_errorMessage;
    QString m_msgTitle;

signals:
    void entitiesModelChanged();
    void filesAndFoldersLoadingFinished();
    void operationError();
    void operationSuccess();
};

#endif // FOLDERPAGECONTROLLER_H
