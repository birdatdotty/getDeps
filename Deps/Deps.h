#ifndef DEPS_H
#define DEPS_H

#include <QObject>
#include <QSet>


class Dll;
class Deps;

class Deps: public QObject
{
    Q_OBJECT
private:
    QSet<QString> libraryNames;
    QMap<QString,Dll*> *library;
    QStringList searchList;
    QStringList foundLibrary,
                foundLibraryPath,
                dontFindLibrary;

    QStringList _deps(QString file);

public:
    Q_INVOKABLE Deps();
    Q_INVOKABLE QStringList deps(QString file);
    QStringList locate(QString file, QStringList from);
    QStringList locate(QString file, QString from = "/");
    Q_INVOKABLE QStringList searchNewLibrary(QStringList lines);

    Q_INVOKABLE void addSearchPath(QString newPath);
    Q_INVOKABLE void addSearchPath(QStringList newPath);

    Q_INVOKABLE void print();

    QStringList getDontFindLibrary();
    QStringList getLibraryNames();

    Q_INVOKABLE void copyTo(QString destination);
};

class Dll {
    QString name;
    QString file;
    QSet<QString> deps;

public:
    Dll (QString name, QString path);
    Dll (QString path);

    QString getFile();
    QString getName();
    QStringList getDeps();
    void addDep(QString newDeps);
    void addDep(QStringList newDeps);
};

#endif // DEPS_H
