# GUI клиент WebDav для ОС Аврора

Приложение с графическим пользовательским интерфейсом, которое позволяет взаимодействовать
с удалённым WebDav сервером. Приложение позволяет взаимодействовать с такими сервисами, как
NextCloud, Yandex.Disk и Communigate.  
Для сборки и запуска приложения нужно указать CLIENT_ID и CLIENT_SECRET. Эти значения нужны аутентификации через
протокол OAuth токенов авторизации API Яндекса. Каждый токен предоставляет приложению доступ к
данным пользователя. Получить CLIENT_ID и CLIENT_SECRET можно тут [oauth.yandex.ru](https://oauth.yandex.ru).

## Особенности

Приложение протестировано на наличие следующих возможностей:

* Приложение позволяет авторизоваться через сервисы NextCloud, Яндекс.Диск, Communigate и WebDav.
* Приложение показывает уведомление об ошибке при неверных учетных данных пользователя.
* Приложение показывает удаленные файлы и директории пользователя.
* Приложение позволяет перемещаться по иерархии удаленной файловой системы.
* Приложение отображает данные о каждом файле и директории в удаленном каталоге.
* Приложение позволяет удалять любые файлы и директории с удаленного сервера.
* Приложение позволяет создавать новые директории на удаленном сервере. 
  Имя папки не должно быть пустым или содержать запрещенные символы.
* Приложение позволяет копировать файлы и директории с удаленного сервера на локальное устройство.
* Приложение позволяет перемещать файлы и директории с удаленного сервера на локальное устройство.
* Приложение позволяет копировать файлы и директории с локального устройства на удаленный сервер.
* Приложение позволяет переименовывать файлы и директории на удаленном сервере.
* Приложение позволяет перемещать файлы и директории на удаленном сервере.
* Приложение позволяет кэшировать файлы с удаленного сервера для их локального редактирования.

## WebDav API

Проект использует библиотеку [qwebdavlib](qwebdavlib/), которая реализует WebDav API
для взаимодействия с удалённым сервером.

Используемые методы QWebDav:
* [QWebDav::setConnectionSettings()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdav.h#L99) используется внутри [классов операций](core/operations) для установки параметров подключения к удалённому серверу.
* [QWebDav::setToken()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdav.h#L93) используется внутри большинства [классов операций](core/operations) для выполнения операций с удалённым сервером.
* [QWebDav::mkdir()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdav.h#L126) используется внутри класса [CreateFolderOperation](core/operations/createfolderoperation.cpp) для создания новой директории на удалённом сервере.
* [QWebDav::remove()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdav.h#L129) используется внутри класса [DeleteFileOrDirectoryOperation](core/operations/deletefileordirectoryoperation.cpp) для удаления файлов или директорий на удалённом сервере.
* [QWebDav::get()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdav.h#L119) используется внутри класса [DownloadFileOperation](core/operations/downloadfileoperation.cpp)  для получения файла с удалённого сервера. 
Также этот метод используется внутри класса [DownloadFolderOperation](core/operations/downloadfolderoperation.cpp) для получения директорий.
Также этот метод используется внутри класса [GetUserInfoOperation](core/operations/getuserinfooperation.cpp) для получения информации о текущем авторизованном пользователе.
* [QWebDav::move()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdav.h#L128) используется внутри класса [MoveFileOrDirectoryOperation](core/operations/movefileordirectoryoperation.cpp) для перемещения файлов и директорий на удалённом сервере. 
Также этот метод используется внутри класса [RenameFileOrDirectoryOperation](core/operations/renamefileordirectoryoperation.cpp) для переименования файлов и директорий.
* [QWebDav::put()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdav.h#L123) используется внутри классов [UploadFileOperation](core/operations/uploadfileoperation.cpp) и [UploadFolderOperation](core/operations/uploadfolderoperation.cpp) для загрузки файлов и директорий на удалённый сервер.
* [QWebdavDirParser::listDirectory()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdavdirparser.h#L76) используется внутри класса [DownloadFolderOperation](core/operations/downloadfolderoperation.cpp) для получения списка вложенных директорий для рекурсивной загрузки.
* [QWebdavDirParser::getFileInfo()](https://gitlab.com/omprussia/examples/WebDavClient/-/blob/example/qwebdavlib/qwebdavdirparser.h#L80) используется внутри класса [FileInfoOperation](core/operations/fileinfooperation.cpp) для получения информации о файле.


## Рекомендации по разработке

При разработке следует учитывать следующие рекомендации.

* [Соглашения об исходном коде](docs/coding_conventions.md)

## Варианты использования

С вариантами использования приложения можно ознакомиться в [руководстве по использованию приложения](docs/manual.ru.md).

## Условия использования и участия

Исходный код проекта предоставляется по [лицензии](LICENSE.BSD-3-Clause.md),
которая позволяет использовать его в сторонних приложениях.

[Соглашение участника](CONTRIBUTING.md) регламентирует права,
предоставляемые участниками компании «Открытая Мобильная Платформа».

Информация об участниках указана в файле [AUTHORS](AUTHORS.md).

[Кодекс поведения](CODE_OF_CONDUCT.md) — это действующий набор правил
компании «Открытая Мобильная Платформа»,
который информирует об ожиданиях по взаимодействию между членами сообщества при общении и работе над проектами.

## Структура проекта

Проект имеет стандартную структуру приложения на базе C++ и QML для ОС Аврора.

* Файл **[ru.auroraos.AuroraOsWebDavGuiClient.pro](ru.auroraos.AuroraOsWebDavGuiClient.pro)**
  описывает структуру проекта для системы сборки qmake.
* Подпроект **[app](app)** содержит исходный код приложения:
    * Файл **[app.pro](app/app.pro)** описывает структуру проекта для системы сборки qmake
    * Каталог **[icons](app/icons)** содержит иконки приложения для поддерживаемых разрешений экрана.
    * Каталог **[qml](app/qml)** содержит исходный код на QML и ресурсы интерфейса пользователя.
    * Каталог **[components](app/qml/components)** содержит пользовательские компоненты пользовательского интерфейса.
    * Каталог **[cover](app/qml/cover)** содержит реализации обложек приложения.
    * Каталог **[dialogs](app/qml/dialogs)** содержит диалоги приложения.
    * Каталог **[icons](app/qml/icons)** содержит дополнительные иконки интерфейса пользователя.
    * Каталог **[pages](app/qml/pages)** содержит страницы приложения.
    * Файл **[AuroraOsWebDavGuiClient.qml](app/qml/AuroraOsWebDavGuiClient.qml)** предоставляет реализацию окна приложения.
  * Каталог **[src](app/src)** содержит исходный код на C++.
    * Файл **[main.cpp](app/src/main.cpp)** является точкой входа в приложение.
  * Каталог **[translations](app/translations)** содержит файлы перевода интерфейса пользователя.
  * Файл **[ru.auroraos.AuroraOsWebDavGuiClient.desktop](app/ru.auroraos.AuroraOsWebDavGuiClient.desktop)** определяет отображение и параметры запуска приложения.
  * Файл **[resources.qrc](app/resources.qrc)** описывает ресурсы приложения.
* Подпроект **[core](core)** содержит исходный код основной библиотеки:
  * Файл **[core.pro](core/core.pro)** описывает структуру подпроекта для системы сборки qmake.
  * Каталог **[controllers](core/controllers)** содержит C++ классы контроллеров приложения.
  * Каталог **[models](core/models)** содержит C++ классы моделей приложения.
  * Каталог **[operations](core/operations)** содержит C++ классы операций приложения.
  * Каталог **[types](core/types)** содержит C++ классы с пользовательскими типами.
  * Каталог **[utils](core/utils)** содержит C++ классы утилит.
* Каталог **[docs](docs)** содержит документацию по проекту:
  * Файл **[build.md](docs/build.md)** описывает сборки приложения.
  * Файл **[coding_conventions.md](docs/coding_conventions.md)** описывает соглашение по разработке.
* Подпроект **[libs](libs)** содержит внешние библиотеки приложения:
  * Файл **[libs.pro](libs/libs.pro)** описывает структуру подпроекта для системы сборки qmake.
* Подпроект **[qwebdavlib](qwebdavlib/)** содержит исходный код библиотеки QWebDav.
* Каталог **[rpm](rpm)** содержит настройки сборки rpm-пакета.
  * Файл **[ru.auroraos.AuroraOsWebDavGuiClient.spec](rpm/ru.auroraos.AuroraOsWebDavGuiClient.spec)** используется инструментом rpmbuild.

## Сборка проекта

Инструкция по сборке приложения находится в файле [docs/build.ru.md](docs/build.ru.md).

## Снимки экранов

![screenshots](screenshots/screenshots.png)

## This document in English

- [README.md](README.md)
