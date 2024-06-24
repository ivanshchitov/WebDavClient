# Application building

## Application structure

The application is build using the qmake build system. Therefore the application is structured as
a set of projects.

* Core project contains set of subprojects.
* The [core](../core/) subproject contains the C++ classes of the application. They are exported
  to the QML context of the application. They are built as the static library to ease their
  use in the unit tests.
* The [app](../app/) subproject contains the bootstrap code and the QML code for the application.
* The [libs](../libs/) subproject contains tools to correctly package the shared libraries into the 
  RPM package.
* The [qwebdavlib](../qwebdavlib/) subproject contains the code of the QWebDavLib that implements
  the interaction over the WebDAV protocol.
* The [catch2](../catch2/) subproject allows to build the Catch2 library to manage tests.
* The [tests](../tests/) subproject contains unit tests for the application.

## Loading git submodules

The application depends on a series of external git repositories.

* [qwebdavlib](https://github.com/catchorg/Catch2) — the QWebDAV library.
* [Catch2](https://github.com/catchorg/Catch2) — the unit test libraries.

Before building the application both libraries should be loaded:

```
git submodule init
git submodule update
```
## Building application

When building the application you must specify Yandex application client id and client secret used for requesting authorization tokens. Application can be registered by following the instructions at https://yandex.ru/dev/disk/doc/dg/concepts/quickstart.html#quickstart__oauth (Note! Specify "Data access" as "яндекс.диск" and select "Доступ к Яндекс.Диску для приложений yadisk:disk").

To build the application with the QtCreator pass client id and client secret as a parameters for a qmake step (Project settings > Build > qmake > Additonal arguments) like this `"CLIENT_ID=..." "CLIENT_SECRET=..."` where `...` is your client id and client secret respectively.

To build the application with the AppTool pass client id and client secret as environment variables. This might be done in two ways:
1. Specify them before the `apptool` keyword when you start building. For example:
```
CLIENT_ID=<your_token> CLIENT_SECRET=<your_token> apptool build
```
2. Run the apptool docker image with the `--env CLIENT_ID=<your_token> --env CLIENT_SECRET=<your_token>` arguments. For example:
```
docker run -it -u `id -u`:`id -g` --rm --env CLIENT_ID=<your_token> --env CLIENT_SECRET=<your_token> -v "$PWD":/sources -w /sources registry.omprussia.ru/omp-sdk/sdk-build-tools:dp2
```

To build the application with the Platform SDK pass client id and client secret as a environment variables:  
Run Platform SDK: $PSDK_DIR/sdk-chroot  
export CLIENT_ID=c24eecb6677c4ed1a38aacc835878d0c  
export CLIENT_SECRET=df7fb0c709664470bd3f8995e34ee2cd  
mb2 -t Aurora_OS-Target build  

## Deploying on the device

The application is packaged into two RPM packages. The base package contains the application
and resources. The tests package contains the unit tests executable.

The tests package does not correspond with the validation rules. In order to install and run tests 
onto the mobile device, the validation must be turned off.

In order to test validation rules for the core application the tests package must be removed
from the list of installed packages:

1. Open the deployment rules in the Qt Creator application.
2. Modify the *RPM Deploy* step. Add the `'-*tests*'` argument to the `sfdk deploy` command.
