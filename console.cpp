#include "console.h"

#include <QCoreApplication>
#include <QDebug>

Console::Console(QObject *parent)
    : QObject(parent)
{
}

void Console::log(QString msg)
{
    qInfo() << msg;
}

