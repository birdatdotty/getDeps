#include "Deps.h"

#include <QProcess>
#include <QSet>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

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

QStringList Deps::buildDll(QString libName)
{
    if (ignoreLibrary.contains(libName))
        return QStringList();

    QStringList ret;

    bool contains = library->keys().contains(libName);
    QStringList deps;

    if (contains)
        deps = library->value(libName)->getDeps();
    else
        deps = Deps::locate(libName, searchList);

    for (QString line: deps) {
        Dll* dll = new Dll(line);
        QString dllName = dll->getName();
        library->insert(dllName,dll);
        foundLibrary.append(dllName);

        ret.append(_deps(dll->getFile()));
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
                if (!dontFindLibrary.contains(lib))
                  if (!ignoreLibrary.contains(lib)) {
                      bool contains = library->keys().contains(lib);
                      QStringList deps;
                      if (contains) deps = library->value(lib)->getDeps();
                      else deps = Deps::locate(lib, searchList);
                      if (!foundLibrary.contains(lib))
                          if (!foundLibraryPath.contains(lib))
                              if (!dontFindLibrary.contains(lib))
                                  newLibrary += buildDll(lib);
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
    qInfo() << __LINE__ << "searchList:" << searchList;
    qInfo() << __LINE__ << "getLibraryNames():" << getLibraryNames();
    qInfo() << __LINE__ << "library->keys():" << library->keys();
    qInfo() << __LINE__ << "dontFindLibrary:" << dontFindLibrary;
    qInfo() << __LINE__ << "ignoreLibrary:" << ignoreLibrary;
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





void Deps::saveJSONLibrary(QString file)
{
     qInfo() << "void Deps::saveJSONLibrary()";
     qInfo() << library->size();
     QJsonObject mainJson;
     for (QString dllName: library->keys()) {
         const Dll * dll = library->find(dllName).value();
         QJsonObject jsonDll;
         QJsonArray jsonDeps;
         for (QString dep: dll->getDeps())
             jsonDeps.append(dep);

         jsonDll["name"] = dllName;
         jsonDll["file"] = dll->getFile();
         jsonDll["deps"] = jsonDeps;
         if (dll->getType() == Dll::ignore)
             jsonDll["type"] = "ignore";
         else
             jsonDll["type"] = "exist";

//        qInfo() << dllName + ":" << dll->getDeps();
//        qInfo() << dllName << jsonDll;
         qInfo() << __LINE__ << jsonDll;
         mainJson[dllName] = jsonDll;
     }
     qInfo() << __LINE__ << mainJson;
     QJsonDocument doc (mainJson);
     doc.toJson();

     QFile jsonFile(file);
     jsonFile.open(QIODevice::WriteOnly);
     jsonFile.write(doc.toJson());
     jsonFile.close();
}

void Deps::loadJSONLibrary(QString file)
{
     qInfo() << "void Deps::loadJSONLibrary(QString file)";
     QFile jsonFile(file);
     jsonFile.open(QIODevice::ReadOnly);
     QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll());
     QJsonObject mainJson = doc.object();
     QStringList libs = mainJson.keys();
     for (QString lib: libs)
     {
         QString name = mainJson[lib].toObject()["name"].toString();
         QString file = mainJson[lib].toObject()["file"].toString();
         QString type = mainJson[lib].toObject()["type"].toString("exist");
         QStringList deps;
         QJsonArray jsonDeps = mainJson[lib].toObject()["deps"].toArray();
         for (int i = 0; i < jsonDeps.size(); i++)
             deps.append(jsonDeps[i].toString());

         qInfo() << name;
         qInfo() << file;
         qInfo() << deps;

         if (type == "ignore") {
             ignoreLibrary.append(name);
         }
         else {
             Dll* dll = new Dll(file);
             dll->addDep(deps);

             library->insert(name,dll);
         }
     }

}
