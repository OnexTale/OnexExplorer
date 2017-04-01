#include "ImageConverter.h"

QImage ImageConverter::convertRGBA4444(QByteArray &array, int width, int height)
{
    QImage img(width, height, QImage::Format_ARGB32);

    img.fill(Qt::transparent);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            unsigned char gb = array.at(y * 2 * height + x * 2);
            unsigned char ar = array.at(y * 2 * height + x * 2 + 1);

            unsigned char g = gb >> 4;
            unsigned char b = gb & 0xF;
            unsigned char a = ar >> 4;
            unsigned char r = ar & 0xF;

            img.setPixel(x, y, qRgba(r * 0x11, g * 0x11, b * 0x11, a * 0x11));
        }
    }

    return img;
}

QImage ImageConverter::convertBGRA8888(QByteArray &array, int width, int height)
{
    QImage img((uchar*)array.data(), width, height, QImage::Format_RGBA8888);

    return img.rgbSwapped();
}

QImage ImageConverter::convertRGB555(QByteArray& array, int width, int height)
{
    QImage img((uchar*)array.data(), width, height, QImage::Format_RGB555);

    return img;
}

ImageConverter::ImageConverter()
{

}

QImage ImageConverter::getFromData(QByteArray &array, int width, int height, ImageConverter::ImageTypes format)
{
    switch (format) {
    case BGRA8888:
        return convertBGRA8888(array, width, height);
        break;
    case RGB555:
        return convertRGB555(array, width, height);
        break;
    case RGBA4444:
        return convertRGBA4444(array, width, height);
        break;

    default:
        break;
    }
}
