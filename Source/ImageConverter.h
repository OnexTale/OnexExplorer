#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H
#include <QImage>
#include <QMap>
#include <QColor>

class ImageConverter
{
private:
public:
    QImage convertGBAR4444(QByteArray& array, int width, int height, int startByte = 0);
    QImage convertBGRA8888(QByteArray& array, int width, int height, int startByte = 0);
    QImage convertARGB555(QByteArray& array, int width, int height, int startByte = 0);
    QImage convertNSTC(QByteArray& array, int width, int height, int startByte = 0);
    QImage convertBGRA8888_INTERLACED(QByteArray& array, int width, int height, int startByte = 0);
    QImage convertBARG4444(QByteArray& array, int width, int height, int startByte = 0);
    enum ImageTypes
    {
        GBAR4444 = 0,
        ARGB555 = 1,
        BGRA8888 = 2,
        NSTC = 3,
        BGRA8888_INTERLACED = 4,
        BARG4444 = 5
    };
    ImageConverter();
};

#endif // IMAGECONVERTER_H
