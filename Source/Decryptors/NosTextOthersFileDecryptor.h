#ifndef NOSTEXTOTHERSFILEDECRYPTOR_H
#define NOSTEXTOTHERSFILEDECRYPTOR_H
#include "INosDecryptor.h"

class NosTextOthersFileDecryptor : public INosDecryptor
{
public:
    NosTextOthersFileDecryptor();
    virtual QByteArray encrypt(QByteArray& array);
    virtual QByteArray decrypt(QByteArray& array);
};

#endif // NOSTEXTOTHERSFILEDECRYPTOR_H
