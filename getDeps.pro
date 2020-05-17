TEMPLATE = app
CONFIG += console c++17
QT += core qml

SOURCES += \
        main.cpp

SOURCES += console.cpp
HEADERS += console.h

include (Deps/Deps.pri)
