# WebDAV Client user manual

## Creating and working with application accounts

An account system is used in the application.
Each application account is linked to its WebDAV server and the remote account of this WebDAV server.
There are four types of accounts available in the application:

1. NextCloud account.
2. YandexDisk account.
3. Communigate account.
4. Account of any WebDAV server.

### Creating an application account

To create an account, follow these steps:

1. Pull down to open menu on the start screen of the application, select «Add account».
2. Select one of the four account types and click «Continue».
3. Enter the required credentials and click «Create».

If an unsuccessful attempt to create an account is made, you will be redirected to entering credentials, and an error notification will appear.

#### Creating a NextCloud application account

To create an application NextCloud account, you must enter the following information:

- Domain address of the NextCloud server, for example, `example.com `.
- User name of the NextCloud account, for example, `user`.
- Password for the NextCloud account.

To learn more about the process of accessing the NextCloud server via WebDAV, check the [documentation](https://docs.nextcloud.com/server/25/user_manual/en/files/access_webdav.html).

#### Create application account for any other WebDAV server

To create an application account for any WebDAV server, you must enter the following information:

- Full link to the WebDAV server.
- User name.
- Password.

### Actions with existing application accounts

A list of the created application accounts can be seen on the start screen of the application.
You can perform the following actions with each of them:

1. Log into your account. To do this, click on the desired account.
2. Delete the account. To do this, click on the desired account and hold, and then click «Delete» in the context menu that appears.

## Working with remote WebDAV storage files and directories

After successful account creation, you will be redirected to the screen with the contents of the WebDAV server root directory.
The name of the current directory is located at the top of the screen.
Below it is a list of files and directories located in the current directory.

To go to a subdirectory, click on it in the list.
You can also return to the start screen of the application from the root directory of the storage using the system transition to the previous screen.
This will log out of the application account.

### Actions with current directory

Actions with the current directory are located in the upper pull-out menu:

1. Refresh the current directory. To do this, click on the «Refresh folder» button. After clicking, the list of files and directories in the current directory will be updated.
2. Create a subdirectory. To do this, click on the «Create folder» button. After clicking, you will be redirected to the dialog for creating an empty nested directory, in which you must enter the name of the directory and click «Create». After creation, you will be redirected to the created folder.
3. Upload file to server. To do this, click on the «Upload file» button. After clicking, you will be redirected to the file selection dialog on the local device. In the dialog, you need to click on the desired file to download.
4. Upload directory to server. To do this, click on the «Upload folder to server» button. After clicking, you will be redirected to the directory selection dialog on the local device. In the dialog box, select the desired directory and click on the «Select folder» button.

### Contextual actions with files/directories in the current directory

To access the context menu with actions on the desired file/directory, click on it and hold it down.
The following actions are available for each file or directory in the current directory:

1. Renaming. To do this, click on the «Rename» button. After clicking, you will be redirected to the file/directory renaming dialog, where you need to enter a new file/directory name and click «Rename».
2. Moving on server. To do this, click on the «Move» button. After clicking, you will be redirected to the file/directory moving dialog, where you can select any directory to move in the WebDAV server storage. But when moving a directory, it is impossible to move it into itself.
3. Deleting. To do this, click on the «Delete» button. After clicking, the selected file/directory on the remote server will be removed, after which the list of files/directories in the current directory will be updated.
4. Downloading. To do this, click on the «Download» button. After clicking, you will be redirected to the dialog for selecting a directory on the local device for downloading a remote file/directory to the device.
5. Moving to device. To do this, click on the «Move to device» button. After clicking, you will be redirected to the dialog for selecting a directory on the local device for downloading a remote file/directory to the device. Unlike downloading, after execution, the selected file/directory will be deleted from the remote server.
6. Editing. Available only for files. To do this, click on the "Edit" button. After clicking, you will be redirected to a dialog in which the file will be downloaded, after which you will be prompted to open an external application for editing the file, if it is installed on the device. After editing, you can save your changes to the server or undo them by accepting or rejecting the dialog.
