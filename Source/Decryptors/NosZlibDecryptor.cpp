#include "NosZlibDecryptor.h"

NosZlibDecryptor::NosZlibDecryptor()
{

}

QByteArray NosZlibDecryptor::encrypt(QByteArray &array)
{
    return qCompress(array);
}

QByteArray NosZlibDecryptor::decrypt(QByteArray &array)
{
    return qUncompress(array);
}
