#include "NosTextOthersFileDecryptor.h"

NosTextOthersFileDecryptor::NosTextOthersFileDecryptor()
{

}

QByteArray NosTextOthersFileDecryptor::encrypt(QByteArray &array)
{
    return QByteArray();
}

QByteArray NosTextOthersFileDecryptor::decrypt(QByteArray &array)
{
    QByteArray result;

    for (auto& byte : array)
        result.push_back(byte ^ 0x1);

    return result;
}
