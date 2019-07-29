#ifndef NOSZLIBOPENER_H
#define NOSZLIBOPENER_H
#include "../Decryptors/NosZlibDecryptor.h"
#include "../Converters/ImageConverter.h"
#include "../NosEnumTypes.h"
#include "INosFileOpener.h"
#include <QDebug>
#include <iostream>
class NosZlibOpener : public INosFileOpener {
private:
    static const int NOS_HEADER_SIZE = 0x10;
    NosZlibDecryptor decryptor;
    QByteArray toBigEndian(qint32 value);
    int getNTHeaderNumber(QByteArray &array);

    OnexTreeItem *createItemFromHeader(QByteArray header, QString name, QByteArray &array, int fileId = -1,
                                       int creationDate = 0, bool compressed = 0);

public:
    NosZlibOpener();
    virtual OnexTreeItem *decrypt(QFile &file);
    virtual QByteArray encrypt(OnexTreeItem *item);
};

#endif // NOSZLIBOPENER_H
