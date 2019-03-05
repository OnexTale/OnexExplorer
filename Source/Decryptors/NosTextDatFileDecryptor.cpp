#include "NosTextDatFileDecryptor.h"

std::vector<unsigned char> NosTextDatFileDecryptor::getMask(QByteArray &array)
{
    std::vector<unsigned char> mask(array.size(), 0x30);
    for (std::size_t i = 0; i < mask.size(); i++)
    {
        unsigned char ch = array[i];
        if (!ch)
            break;

        if (ch -= 0x20)
        {
            ch -= 0xD;

            if (!(ch < 2))
            {
                ch -= 0x3;
                bool test1 = ch < 0xA;
                ch -= 0xA;
                if (!test1 && ch != 0xC5)
                    continue;
            }
        }

        mask[i] = 0x31;
    }

    return mask;
}

NosTextDatFileDecryptor::NosTextDatFileDecryptor() :
    cryptoArray({ 0x00, 0x20, 0x2D, 0x2E, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x0A, 0x00 })
{

}

QByteArray NosTextDatFileDecryptor::encrypt(QByteArray &array)
{
    std::vector<unsigned char> mask = getMask(array);
    QByteArray string;

    int iterator = 0;
    int size = mask.size();
    unsigned char switchByte = 0;

    while (iterator < size)
    {
        int len = iterator;
        while ((iterator < mask.size() && mask.at(iterator) == 0x30) || (iterator + 1 == mask.size()) || (iterator + 2 == mask.size()) || (iterator + 1 < mask.size() && mask.at(iterator + 1) == 0x30) || (iterator + 2 < mask.size() && mask.at(iterator + 2) == 0x30))
            ++iterator;

        if (iterator > len)
        {
            for (int j = iterator - len; j > 0; j -= 0x7E)
            {
                int checker = j;
                if (checker > 0x7E) {
                    checker = 0x7E;
                }

                string.push_back(checker);
                for (; checker > 0; --checker)
                {
                    unsigned char byteToAdd = array[len] ^ 0x33;
                    len++;
                    string.push_back(byteToAdd);
                }
            }
        }

        if (iterator >= size)
            break;

        len = iterator;
        int v25 = 1;

        while (iterator < mask.size() && mask.at(iterator) == 0x31)
            ++iterator;

        if (iterator > len)
        {
            for (int j = iterator - len; j > 0; j -= 0x7E)
            {
                int checker = j;
                if (j > 0x7E)
                    checker = 0x7E;

                string.push_back(checker | 0x80);
                for (; checker > 0; --checker)
                {
                    unsigned char byteToAdd = array[len];
                    len++;

                    switch (byteToAdd)
                    {
                    case 32:
                        switchByte = 1;
                        break;
                    case 45:
                        switchByte = 2;
                        break;
                    case 46:
                        switchByte = 3;
                        break;
                    case 0xFF:
                        switchByte = 14;
                        break;
                    default:
                        switchByte = byteToAdd - 0x2C;
                        break;
                    }

                    if (v25)
                    {
                        string.push_back(0x10 * switchByte);
                        v25 = 0;
                    }
                    else
                    {
                        string[string.size() - 1] = string[string.size() - 1] | switchByte;
                        v25 = 1;
                    }
                }
            }
        }
    }

    string.push_back(0xFF);

    return string;
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
            for (; validate > 0; --validate)
            {
                if (currIndex >= array.size())
                    break;

                currentByte = array.at(currIndex);
                currIndex++;

                decryptedFile.push_back(currentByte ^ 0x33);
            }
        }
    }

    return decryptedFile;
}

NosTextDatFileDecryptor::~NosTextDatFileDecryptor()
{

}
