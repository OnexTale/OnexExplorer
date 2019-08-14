#ifndef NOSZLIBDECRYPTOR_H
#define NOSZLIBDECRYPTOR_H

#include "INosDecryptor.h"
#include <QByteArray>

class NosZlibDecryptor {
public:
    NosZlibDecryptor();
    QByteArray encrypt(QByteArray &array, int level = 1);
    QByteArray decrypt(QByteArray &array);
};

#endif // NOSZLIBDECRYPTOR_H
