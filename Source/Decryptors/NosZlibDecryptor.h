#ifndef NOSZLIBDECRYPTOR_H
#define NOSZLIBDECRYPTOR_H
#include "INosDecryptor.h"
#include <QByteArray>

class NosZlibDecryptor {
    public:
    NosZlibDecryptor();
    virtual QByteArray encrypt(QByteArray &array, int level = 1);
    virtual QByteArray decrypt(QByteArray &array);
};

#endif // NOSZLIBDECRYPTOR_H
