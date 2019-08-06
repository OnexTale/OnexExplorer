#include "IModelConverter.h"
#include <QtEndian>

short IModelConverter::fromLittleEndianToShort(QByteArray array) {
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

int IModelConverter::fromLittleEndianToInt(QByteArray array) {
    return qFromLittleEndian<qint32>(reinterpret_cast<const uchar *>(array.data()));
}

float IModelConverter::fromLittleEndianToFloat(QByteArray array) {
    return qFromLittleEndian<float>(reinterpret_cast<const uchar *>(array.data()));
}

QByteArray IModelConverter::fromShortToLittleEndian(short number) {
    QByteArray writeArray;
    writeArray.resize(2);
    qToLittleEndian<qint16>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray IModelConverter::fromIntToLittleEndian(int number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<qint32>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray IModelConverter::fromFloatToLittleEndian(float number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<float>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}