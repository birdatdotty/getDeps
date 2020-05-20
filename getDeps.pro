TEMPLATE = app
CONFIG += console c++17
QT += core qml

SOURCES += \
        main.cpp

include (Deps/Deps.pri)
include (Console/Console.pri)
