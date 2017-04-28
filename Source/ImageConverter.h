#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H
#include <QImage>
#include <QMap>
#include <QColor>

class ImageConverter
{
private:
    QImage convertGBAR4444(QByteArray& array, int width, int height);
    QImage convertBGRA8888(QByteArray& array, int width, int height);
    QImage convertARGB555(QByteArray& array, int width, int height);
    QImage convertNSTC(QByteArray& array, int width, int height);
    QImage convertBGRA8888_INTERLACED(QByteArray& array, int width, int height);
    QImage convertBARG4444(QByteArray& array, int width, int height);
public:
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
    QImage getFromData(QByteArray& array, int width, int height, ImageTypes format);
};

#endif // IMAGECONVERTER_H
