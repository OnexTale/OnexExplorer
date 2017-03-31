#include "INosFileOpener.h"

int INosFileOpener::readNextInt(QFile &file)
{
    return qFromLittleEndian<int>(reinterpret_cast<const uchar *>(file.read(4).data()));
}

QString INosFileOpener::neatFileName(QString &fileName)
{
    QStringList list = fileName.split("/");

    if (list.empty())
        return fileName;

    return list.back();
}
