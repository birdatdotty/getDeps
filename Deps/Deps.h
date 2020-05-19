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
    QString startDLLName;
    int startDLLNameLen;
    QStringList searchList;
    QStringList foundLibrary,
                foundLibraryPath,
                dontFindLibrary,
                ignoreLibrary;

    QStringList _deps(QString file);
    QStringList buildDll(QString libName);

public:
    Q_INVOKABLE Deps();
    Q_INVOKABLE QStringList deps(QString file);
    QStringList locate(QString file, QStringList from);
    QStringList locate(QString file, QString from = "/");
    Q_INVOKABLE QStringList searchNewLibrary(QStringList lines);

    Q_INVOKABLE void setDLLName(QString newDLLName);

    Q_INVOKABLE void addSearchPath(QString newPath);
    Q_INVOKABLE void addSearchPath(QStringList newPath);

    Q_INVOKABLE void print();

    QStringList getDontFindLibrary();
    QStringList getLibraryNames();

    Q_INVOKABLE void copyTo(QString destination);
    Q_INVOKABLE void saveJSONLibrary(QString file);
    Q_INVOKABLE void loadJSONLibrary(QString file);
};

class Dll {
  public:
    enum Type {exist, ignore};
    Dll (QString name, QString path);
    Dll (QString path, Dll::Type t = exist);

    QString getFile() const;
    QString getName() const;
    QStringList getDeps() const;
    Type getType() const;
    void addDep(QString newDeps);
    void addDep(QStringList newDeps);

  private:
    QString name;
    QString file;
    Type type;
    QSet<QString> deps;
};

#endif // DEPS_H
