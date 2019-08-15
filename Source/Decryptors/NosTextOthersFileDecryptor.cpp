#include "NosTextOthersFileDecryptor.h"

NosTextOthersFileDecryptor::NosTextOthersFileDecryptor(LittleEndianConverter *littleEndianConverter) {
    this->littleEndianConverter = littleEndianConverter;
}

QByteArray NosTextOthersFileDecryptor::encrypt(QByteArray &array) {
    QByteArray result = littleEndianConverter->toInt(array.size());
    for (auto &byte : array) {
        result.push_back(byte ^ 0x1);
    }
    return result;
}

QByteArray NosTextOthersFileDecryptor::decrypt(QByteArray &array) {
    QByteArray result;
    int lines = littleEndianConverter->fromInt(array.mid(0, 4));
    int pos = 4;
    for (int i = 0; i < lines; i++) {
        int strLen = littleEndianConverter->fromInt(array.mid(pos, 4));
        pos += 4;
        QByteArray str = array.mid(pos, strLen);
        pos += strLen;
        for (auto &byte : str)
            result.push_back(byte ^ 0x1);
        result.push_back('\n');
    }
    return result;
}
