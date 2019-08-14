#ifndef INOSDECRYPTOR_H
#define INOSDECRYPTOR_H

#include <QByteArray>

class INosDecryptor {
public:
    virtual QByteArray encrypt(QByteArray &array) = 0;
    virtual QByteArray decrypt(QByteArray &array) = 0;
};

#endif // INOSDECRYPTOR_H
