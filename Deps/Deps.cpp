#include "Deps.h"

#include <QProcess>
#include <QSet>
#include <QDir>

#define START_WITH "\tDLL Name: "
//#define START_WITH "\tИмя DLL: "

#include <QDebug>

QStringList Deps::_deps(QString file)
{
    qInfo() << "QStringList Deps::_deps(QString file)" << file;
    qInfo() << "file:" << file;
    QStringList ret;
    QProcess proc;
    QString cmd = "objdump";
    QStringList lines;
    QString out;

    QStringList cmdOpts = QStringList() << "-x" << file;

    connect(&proc, &QProcess::readyReadStandardOutput,
            [&proc, &out] () {
        out += proc.readAllStandardOutput();
    });
    proc.start(cmd, cmdOpts);
    proc.waitForFinished(-1);

    lines += out.split("\n");

    for (QString line: lines) {
        if (line.startsWith(startDLLName))
        {
            ret.append(line.mid(startDLLNameLen));
        }
        if (line.endsWith("dll")) {
            qInfo() << "line:" << line;
            qInfo() << "line:" << line.mid(startDLLNameLen);
        }
    }

    return ret;
}

Deps::Deps()
{
    library = new QMap<QString,Dll*>;
    startDLLName = "\tDLL Name: ";
    startDLLNameLen = startDLLName.size();
}

QStringList Deps::deps(QString file)
{
    QStringList ret = _deps(file);

    QStringList newlib_ = searchNewLibrary(ret);
    while (newlib_.size() != 0) {
        newlib_ = searchNewLibrary(newlib_);
    }

    return ret;
}

QStringList Deps::locate(QString file, QStringList from)
{
    QProcess proc;
    QString cmd = "locate";
    QString out;
    QStringList lines;
    QSet<QString> ret;

    connect(&proc, &QProcess::readyReadStandardOutput,
            [&proc, &out] () {
        out += proc.readAllStandardOutput();
    });
    proc.start(cmd, QStringList() << file);
    proc.waitForFinished(-1);

    lines += out.split("\n");
    for (QString line: lines)
        for (QString fromI: from)
            if (line.startsWith(fromI))
                ret << line;

    QStringList listRet = ret.toList();

    if (ret.size() == 0)
        dontFindLibrary.append(file);
    else {
        libraryNames.insert(file);
        foundLibraryPath.append(listRet.at(0));
    }

    return ret.toList();

}

QStringList Deps::locate(QString file, QString from)
{
    return locate(file, QStringList() << from);
}

QStringList Deps::searchNewLibrary(QStringList libs) {
    QStringList newLibrary;

    for (QString lib: libs) {
        if (searchList.size() == 0)
            searchList << "/";

        qInfo() << "lib:" << lib;
        qInfo() << "foundLibrary:" << foundLibrary;
        qInfo() << "foundLibraryPath:" << foundLibraryPath;
        qInfo() << "dontFindLibrary:" << dontFindLibrary;

        if (!foundLibrary.contains(lib))
            if (!foundLibraryPath.contains(lib))
                if (!dontFindLibrary.contains(lib)) {
                    QStringList deps = Deps::locate(lib, searchList);
                    for (QString line: deps) {
                        Dll* dll = new Dll(line);
                        QString dllName = dll->getName();
                        library->insert(dllName,dll);
                        foundLibrary.append(dllName);

//                        newLibrary.append(dllName);
                        newLibrary.append(_deps(dll->getFile()));
                    }
                }
    }

    return newLibrary;
}

void Deps::setDLLName(QString newDLLName) {
    startDLLName = newDLLName;
    startDLLNameLen = newDLLName.size();
}

void Deps::addSearchPath(QString newPath) {
    searchList.append(newPath);
}

void Deps::addSearchPath(QStringList newPath) {
    searchList.append(newPath);
}

void Deps::print() {
    qInfo() << "void Deps::print()";
    qInfo() << __LINE__ << searchList;
    qInfo() << __LINE__ << getLibraryNames();
}

QStringList Deps::getDontFindLibrary() {
    return dontFindLibrary;
}

QStringList Deps::getLibraryNames() {
    return libraryNames.toList();
}
void Deps::copyTo(QString destination)
{
    QDir dest(destination);
    qInfo("");
    qInfo() << "copy to " + destination + ":";

    dest.mkpath(destination);
    for (QString dll: foundLibraryPath) {
        qInfo() << "  " + dll;
        QFileInfo fileInfo(dll);
        QFile lib(dll);
        lib.copy(destination + QDir::separator() + fileInfo.fileName());
    }
}

QString Dll::getFile() {
    return file;
}

QString Dll::getName() {
    return name;
}

QStringList Dll::getDeps() {
    return deps.toList();
}

void Dll::addDep(QString newDeps) {
    deps.insert(newDeps);
}

void Dll::addDep(QStringList newDeps)
{
    for (QString newDep: newDeps)
        deps.insert(newDep);
}
