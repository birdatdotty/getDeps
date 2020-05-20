#include "console.h"

#include <QDebug>

#include <Deps.h>
#include <QCoreApplication>
#include <QFile>
#include <QJSEngine>

using namespace std;


int main(int argc, char **argv) {
    if (argc < 2)
    {
        qInfo() << QString("run ") + argv[0] + " script.qs file.exe";
        return 1;
    }

    QCoreApplication a(argc, argv);
    QJSEngine myEngine;

    QString fileName(argv[1]);
    QFile scriptFile(fileName.toUtf8());
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        // handle error
        return 1;
    }


    // deps:
    QJSValue jsMetaObject = myEngine.newQMetaObject(&Deps::staticMetaObject);
    myEngine.globalObject().setProperty("Deps", jsMetaObject);

    // console:
    Console console;
    QJSValue valConsole = myEngine.newQObject(&console);
    myEngine.globalObject().setProperty("console", valConsole);

    // file:
    myEngine.globalObject().setProperty("file", argv[2]);

    // args:
    QJSValue jsArgs = myEngine.newArray(argc);
    for (int i = 0; i < argc; i++)
        jsArgs.setProperty(i, argv[i]);
    myEngine.globalObject().setProperty("args", jsArgs);

    // content:
    QTextStream stream(&scriptFile);
    QString contents;
    QStringList tmp_contents = stream.readAll().split("\n");
    if (tmp_contents.at(0).startsWith("#!"))
        tmp_contents.takeAt(0);

    contents = tmp_contents.join("\n");

    scriptFile.close();
    myEngine.evaluate(contents, fileName);

    return 0;
}
