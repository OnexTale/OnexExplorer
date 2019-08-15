#include "NosCCInfOpener.h"

NosCCInfOpener::NosCCInfOpener() = default;

OnexTreeItem *NosCCInfOpener::decrypt(QFile &file) {
    file.seek(0);
    QByteArray header = file.read(0x10);
    int fileSize = littleEndianConverter.readInt(file);
    littleEndianConverter.readInt(file);   //second time the same bytes, idk why
    file.read(1);   //0x00
    int fileAmount = littleEndianConverter.readInt(file);
    qDebug() << fileAmount;
    return nullptr;
}

QByteArray NosCCInfOpener::encrypt(OnexTreeItem *item) {
    return QByteArray();
}
