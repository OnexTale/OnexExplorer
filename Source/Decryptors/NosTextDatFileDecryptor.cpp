#include "NosTextDatFileDecryptor.h"

NosTextDatFileDecryptor::NosTextDatFileDecryptor() :
    cryptoArray({ 0x00, 0x20, 0x2D, 0x2E, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x0A, 0x00 })
{

}

QByteArray NosTextDatFileDecryptor::encrypt(QByteArray &array)
{
    return QByteArray();
}

QByteArray NosTextDatFileDecryptor::decrypt(QByteArray &array)
{
    QByteArray decryptedFile;
    int currIndex = 0;
    while (currIndex < array.size())
    {
        unsigned char currentByte = array.at(currIndex);
        currIndex++;
        if (currentByte == 0xFF)
        {
            decryptedFile.push_back(0xD);
            continue;
        }

        int validate = currentByte & 0x7F;

        if (currentByte & 0x80)
        {
            for (; validate > 0; validate -= 2)
            {
                if (currIndex >= array.size())
                    break;

                currentByte = array.at(currIndex);
                currIndex++;

                int firstByte = cryptoArray.at((currentByte & 0xF0) >> 4);
                decryptedFile.push_back(firstByte);

                if (validate <= 1)
                    break;
                int secondByte = cryptoArray.at(currentByte & 0xF);

                if (!secondByte)
                    break;

                decryptedFile.push_back(secondByte);
            }
        }
        else
        {
            for (; validate > 0; )
            {
                if (currIndex >= array.size())
                    break;

                currentByte = array.at(currIndex);
                currIndex++;

                decryptedFile.push_back(currentByte ^ 0x33);

                --validate;
            }
        }
    }

    return decryptedFile;
}

NosTextDatFileDecryptor::~NosTextDatFileDecryptor()
{

}
