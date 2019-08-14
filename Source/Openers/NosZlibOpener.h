#ifndef NOSZLIBOPENER_H
#define NOSZLIBOPENER_H

#include "../Decryptors/NosZlibDecryptor.h"
#include "../Converters/ImageConverter.h"
#include "../NosEnumTypes.h"
#include "INosFileOpener.h"
#include <QDebug>
#include <iostream>

class NosZlibOpener : public INosFileOpener {
public:
    NosZlibOpener();
    OnexTreeItem *decrypt(QFile &file);
    QByteArray encrypt(OnexTreeItem *item);
private:
    static const int NOS_HEADER_SIZE = 0x10;
    NosZlibDecryptor decryptor;
    OnexTreeItem *createItemFromHeader(QByteArray header, const QString &name, QByteArray &array, int fileId = -1,
                                       int creationDate = 0, bool compressed = false);
    int getNTHeaderNumber(QByteArray &array);
    QByteArray toBigEndian(qint32 value);
};

#endif // NOSZLIBOPENER_H
