#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include <QColor>
#include <QImage>
#include <QMap>
#include <QtEndian>
#include "LittleEndianConverter.h"

class ImageConverter {
public:
    QImage convertGBAR4444(QByteArray &array, int width, int height, int startByte = 0);
    QImage convertBGRA8888(QByteArray &array, int width, int height, int startByte = 0);
    QImage convertARGB555(QByteArray &array, int width, int height, int startByte = 0);
    QImage convertNSTC(QByteArray &array, int width, int height, int startByte = 0);
    QImage convertBGRA8888_INTERLACED(QByteArray &array, int width, int height, int startByte = 0);
    QImage convertBARG4444(QByteArray &array, int width, int height, int startByte = 0);
    QImage convertGrayscale(QByteArray &array, int width, int height, int startByte = 0);
    QByteArray toGBAR4444(QImage &image);
    QByteArray toNSTC(QImage &image);
    QByteArray toBGRA8888(QImage &image);
    QByteArray toARGB555(QImage &image);
    QByteArray toBGRA8888_INTERLACED(QImage &image);
    QByteArray toBARG4444(QImage &image);
    QByteArray toGrayscale(QImage &image);
    explicit ImageConverter(LittleEndianConverter *littleEndianConverter);
private:
    QMap<int, QColor> colors;
    LittleEndianConverter *littleEndianConverter;
};

#endif // IMAGECONVERTER_H
