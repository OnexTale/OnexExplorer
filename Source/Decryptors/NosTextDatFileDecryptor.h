#ifndef NOSTEXTFILEDECRYPTOR_H
#define NOSTEXTFILEDECRYPTOR_H

#include "INosDecryptor.h"
#include <array>
#include <vector>

class NosTextDatFileDecryptor : public INosDecryptor {
public:
    NosTextDatFileDecryptor();
    ~NosTextDatFileDecryptor();
    QByteArray encrypt(QByteArray &array);
    QByteArray decrypt(QByteArray &array);
private:
    const std::array<unsigned char, 16> cryptoArray;
    std::vector<unsigned char> getMask(QByteArray &array);
};

#endif // NOSTEXTFILEDECRYPTOR_H
