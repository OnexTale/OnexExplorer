#include "LittleEndianConverter.h"
#include <QtEndian>

short LittleEndianConverter::fromShort(QByteArray array) {
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

int LittleEndianConverter::fromInt(QByteArray array) {
    return qFromLittleEndian<qint32>(reinterpret_cast<const uchar *>(array.data()));
}

float LittleEndianConverter::fromFloat(QByteArray array) {
    return qFromLittleEndian<float>(reinterpret_cast<const uchar *>(array.data()));
}

QByteArray LittleEndianConverter::toShort(short number) {
    QByteArray writeArray;
    writeArray.resize(2);
    qToLittleEndian<qint16>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray LittleEndianConverter::toInt(int number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<qint32>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray LittleEndianConverter::toFloat(float number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<float>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

int LittleEndianConverter::readInt(QFile &file) {
    return fromInt(file.read(4));
}

short LittleEndianConverter::readShort(QFile &file) {
    return fromShort(file.read(2));
}