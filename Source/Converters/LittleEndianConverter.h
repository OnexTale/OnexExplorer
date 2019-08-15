#ifndef LITTLEENDIANCONVERTER_H
#define LITTLEENDIANCONVERTER_H

#include <QFile>
#include <QByteArray>

class LittleEndianConverter {
public:
    short fromShort(QByteArray array);
    int fromInt(QByteArray array);
    float fromFloat(QByteArray array);
    QByteArray toShort(short number);
    QByteArray toInt(int number);
    QByteArray toFloat(float number);
    int readInt(QFile &file);
    short readShort(QFile &file);
};

#endif // LITTLEENDIANCONVERTER_H