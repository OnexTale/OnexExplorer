#ifndef NOSTEXTOTHERSFILEDECRYPTOR_H
#define NOSTEXTOTHERSFILEDECRYPTOR_H

#include "INosDecryptor.h"
#include <QDebug>
#include <QtEndian>
#include "../Converters/LittleEndianConverter.h"

class NosTextOthersFileDecryptor : public INosDecryptor {
public:
    NosTextOthersFileDecryptor(LittleEndianConverter *littleEndianConverter);
    QByteArray encrypt(QByteArray &array);
    QByteArray decrypt(QByteArray &array);
private:
    LittleEndianConverter *littleEndianConverter;
};

#endif // NOSTEXTOTHERSFILEDECRYPTOR_H
