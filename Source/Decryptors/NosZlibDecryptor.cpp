#include "NosZlibDecryptor.h"

NosZlibDecryptor::NosZlibDecryptor()
{

}

QByteArray NosZlibDecryptor::encrypt(QByteArray &array)
{
    //QByteArray compressed = qCompress(array);

    return QByteArray();
}

QByteArray NosZlibDecryptor::decrypt(QByteArray &array)
{
    return qUncompress(array);
}
