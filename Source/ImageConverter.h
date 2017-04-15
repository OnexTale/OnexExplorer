#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H
#include <QImage>
#include <QMap>

class ImageConverter
{
private:
    QImage convertRGBA4444(QByteArray& array, int width, int height);
    QImage convertBGRA8888(QByteArray& array, int width, int height);
    QImage convertRGB555(QByteArray& array, int width, int height);
    QImage convertNSTC(QByteArray& array, int width, int height);
public:
    enum ImageTypes
    {
        RGBA4444 = 0,
        RGB555,
        BGRA8888,
        NSTC
    };
    ImageConverter();
    QImage getFromData(QByteArray& array, int width, int height, ImageTypes format);
};

#endif // IMAGECONVERTER_H
