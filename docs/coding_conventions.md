# Coding conventions

The source code of the application must adhere to the Qt coding conventions.

* https://wiki.qt.io/Coding_Conventions
* https://wiki.qt.io/Qt_Coding_Style
* https://doc.qt.io/qt-5/qml-codingconventions.html
* https://doc.qt.io/qt-5/qtwritingstyle-cpp.html

## Applying formatting in Qt Creator

The source code can be formatted using the the QtCreator Beautifier plugin.
The full reference on how to apply it contains in the official 
[QtCreator manual](https://doc.qt.io/qtcreator/creator-beautifier.html).

## Applying the formatting manually

The source code must be formatted with the following tools before committing it
to the VCS:

* [qmlformat](https://doc-snapshots.qt.io/qt6-dev/qtquick-tools-and-utilities.html#qmlformat)
* [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

These tools must be installed in the host operating system.
