#ifndef NOSZLIBOPENER_H
#define NOSZLIBOPENER_H
#include "INosFileOpener.h"
#include "../Decryptors/NosZlibDecryptor.h"
#include <QDebug>
#include <iostream>

class NosZlibOpener : public INosFileOpener
{
private:
    static const int NOS_HEADER_SIZE = 0x10;
    NosZlibDecryptor decryptor;
    QByteArray toBigEndian(qint32 value);
public:
    NosZlibOpener();
    virtual OnexTreeItem* decrypt(QFile& file);
};

#endif // NOSZLIBOPENER_H
