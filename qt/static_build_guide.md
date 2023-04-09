# How to build Qt statically with MSVC 2019

 - Example with Qt 6.5.0
 - Using MSVC and VS2019

Prerequisites:
 - Perl http://www.activestate.com/activeperl
 - Python http://www.python.org/download/

Download Qt source zip from "https://download.qt.io/official_releases/qt/6.5/6.5.0/single/qt-everywhere-src-6.5.0.zip"

Extract, e.g. to `C:\qt-6.5.0-src`

Run `x64 Native Tools Command Prompt for VS 2019`
```
cd C:\qt-6.5.0-src
mkdir C:\qt-6.5.0-static
set QTDIR=C:\qt-6.5.0-src\qtbase
set PATH=C:\qt-6.5.0-src\qtbase\bin;%PATH%
configure -debug-and-release -prefix "C:\qt-6.5.0-static" -opensource -platform win32-msvc -opengl desktop -static -static-runtime -nomake examples -nomake tests
cmake --build . --parallel
ninja install
```

Install the Qt VS Tools extension https://marketplace.visualstudio.com/publishers/TheQtCompany

In Visual Studio `Extensions -> Qt VS Tools -> Qt Options`, add `C:\qt\qt-6.5.0-static` and make it the default.

If working on an existing project, make sure to update `Qt Installation` in `Extensions -> Qt Project Settings`

If you get strange error messages regarding "QT_INSTALL_PREFIX" then navigate to qmake.exe and create a file next to it called "qt.conf" and put
```
[Paths]
Prefix = ..
```
in it. ("C:\qt-6.5.0-static\bin\qt.conf")