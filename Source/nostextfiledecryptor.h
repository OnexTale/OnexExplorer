#ifndef NOSTEXTFILEDECRYPTOR_H
#define NOSTEXTFILEDECRYPTOR_H
#include <array>
#include "inosdecryptor.h"

class NosTextFileDecryptor : public INosDecryptor
{
private:
    const std::array<unsigned char, 16> cryptoArray;
public:
    NosTextFileDecryptor();
    virtual QByteArray encrypt(QByteArray& array) = 0;
    virtual QByteArray decrypt(QByteArray& array) = 0;
    ~NosTextFileDecryptor();
};

#endif // NOSTEXTFILEDECRYPTOR_H
