#include "NosZlibDecryptor.h"

NosZlibDecryptor::NosZlibDecryptor()
{

}

QByteArray NosZlibDecryptor::encrypt(QByteArray &array)
{
    return qCompress(array, 1);
}

QByteArray NosZlibDecryptor::decrypt(QByteArray &array)
{
    return qUncompress(array);
}
