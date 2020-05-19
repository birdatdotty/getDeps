#include "Deps.h"

Dll::Dll(QString name, QString path)
    : name (name),
      file(path)
{

}

Dll::Dll(QString path, Dll::Type t)
    : file (path),
      type(t)
{
    QStringList pathSplit = path.split("/");
    if (pathSplit.size() > 1)
        name = pathSplit.last();
}

QString Dll::getFile() const {
    return file;
}

QString Dll::getName() const {
    return name;
}

QStringList Dll::getDeps() const {
    return deps.toList();
}

Dll::Type Dll::getType() const {
    return type;
}

void Dll::addDep(QString newDeps) {
    deps.insert(newDeps);
}

void Dll::addDep(QStringList newDeps)
{
    for (QString newDep: newDeps)
        deps.insert(newDep);
}
