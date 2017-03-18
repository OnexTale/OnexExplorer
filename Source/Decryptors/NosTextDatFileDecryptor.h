#ifndef NOSTEXTFILEDECRYPTOR_H
#define NOSTEXTFILEDECRYPTOR_H
#include <array>
#include "INosDecryptor.h"

class NosTextDatFileDecryptor : public INosDecryptor
{
private:
    const std::array<unsigned char, 16> cryptoArray;
public:
    NosTextDatFileDecryptor();
    virtual QByteArray encrypt(QByteArray& array) = 0;
    virtual QByteArray decrypt(QByteArray& array) = 0;
    ~NosTextDatFileDecryptor();
};

#endif // NOSTEXTFILEDECRYPTOR_H
