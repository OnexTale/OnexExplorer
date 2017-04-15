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
    enum GridColors
    {
        GRAY = qRgb(100, 100, 100),
        BLACK = qRgb(0, 0, 0),
        WHITE = qRgb(255, 255, 255),
        RED = qRgb(150, 100, 100),
        GREEN = qRgb(100, 150, 100),
        BLUE = qRgb(100, 100, 150),
        YELLOW = qRgb(150, 150, 100),
        PURPLE = qRgb(150, 100, 150),
        H_GREEN = qRgb(0, 200, 50),
        H_BLUE = qRgb(0, 50, 200),
        H_RED = qRgb(200, 50, 0),
        H_YELLOW = qRgb(250, 230, 10)
    };
    ImageConverter();
    QImage getFromData(QByteArray& array, int width, int height, ImageTypes format);
};

#endif // IMAGECONVERTER_H
