#include "NosCCInfOpener.h"

NosCCInfOpener::NosCCInfOpener()
{

}

OnexTreeItem *NosCCInfOpener::decrypt(QFile &file)
{
    file.seek(0);

    QByteArray header = file.read(0x10);

    int fileSize = readNextInt(file);
    readNextInt(file);   //second time the same bytes, idk why
    file.read(1);   //0x00
    int fileAmount = readNextInt(file);
    qDebug() << fileAmount;
    return NULL;
}

QByteArray NosCCInfOpener::encrypt(OnexTreeItem *item)
{
    return QByteArray();
}
