#include "Deps.h"

Dll::Dll(QString name, QString path)
    : name (name),
      file(path)
{

}

Dll::Dll(QString path)
    : file (path)
{
    QStringList pathSplit = path.split("/");
    if (pathSplit.size() > 1)
        name = pathSplit.last();
}
