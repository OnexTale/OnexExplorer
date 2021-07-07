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
    int lines = qFromLittleEndian<int>(array.mid(0, 4));
    int pos = 4;

    for (int i = 0; i < lines; i++)
    {
        int strLen = qFromLittleEndian<int>(array.mid(pos, 4));
        pos += 4;
        QByteArray str = array.mid(pos, strLen);
        pos += strLen;
        for (auto& byte : str)
            result.push_back(byte ^ 0x1);
        result.push_back('\n');
    }
    qDebug() << result;
    return result;
}
