#ifndef NOSTEXTFILEDECRYPTOR_H
#define NOSTEXTFILEDECRYPTOR_H
#include <array>
#include "INosDecryptor.h"
#include <vector>

class NosTextDatFileDecryptor : public INosDecryptor
{
private:
    const std::array<unsigned char, 16> cryptoArray;
    std::vector<unsigned char> getMask(QByteArray &array);
public:
    NosTextDatFileDecryptor();
    virtual QByteArray encrypt(QByteArray& array);
    virtual QByteArray decrypt(QByteArray& array);
    ~NosTextDatFileDecryptor();
};

#endif // NOSTEXTFILEDECRYPTOR_H
