#include "ImageConverter.h"

QImage ImageConverter::convertGBAR4444(QByteArray &array, int width, int height)
///GBAR = ARGB (endianness)
{
    QImage img(width, height, QImage::Format_ARGB32);

    img.fill(Qt::transparent);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uchar gb = array.at(y * 2 * height + x * 2);
            uchar ar = array.at(y * 2 * height + x * 2 + 1);

            uchar g = gb >> 4;
            uchar b = gb & 0xF;
            uchar a = ar >> 4;
            uchar r = ar & 0xF;

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

QImage ImageConverter::convertARGB555(QByteArray& array, int width, int height)
{
    QImage img((uchar*)array.data(), width, height, QImage::Format_RGB555);

    return img;
}

QImage ImageConverter::convertNSTC(QByteArray &array, int width, int height)
{
    QImage img(width, height, QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    QMap<int, QColor> colors;
    colors[-1] = qRgb(150, 0, 255);
    colors[0x0] = qRgb(255, 255, 255);
    colors[0x1] = qRgb(100, 100, 100);
    colors[0x2] = qRgb(100, 150, 100);
    colors[0x3] = qRgb(0, 0, 0);
    colors[0x9] = qRgb(100, 100, 150);
    colors[0x0a] = qRgb(0, 50, 200);
    colors[0x0b] = qRgb(150, 100, 100);
    colors[0x0d] = qRgb(150, 150, 100);
    colors[0x10] = qRgb(150, 100, 150);
    colors[0x11] = qRgb(0, 200, 50);
    colors[0x12] = qRgb(200, 50, 0);
    colors[0x13] = qRgb(250, 230, 10);
    //move this list somewhere else?

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            uchar value = array.at(y * width + x);
            if (colors.contains(value))
                img.setPixel(x, y, colors[value].rgb());
            else
                img.setPixel(x, y, colors[-1].rgb());
        }
    }

    return img.scaled(QSize(width*2, height*2));
}

QImage ImageConverter::convertBGRA8888_INTERLACED(QByteArray &array, int width, int height)
{
    QImage img(width, height, QImage::Format_RGBA8888);
    img.fill(Qt::transparent);

    short num = 0,
            x = 0,
            y = 0;
    for (int i = 0; i < array.size();)
    {
        uchar b = array.at(i++);
        uchar g = array.at(i++);
        uchar r = array.at(i++);
        uchar a = array.at(i++);
        img.setPixel(x, y, qRgba(r, g, b, a));

        x++;
        if (x == (num + 256) || x == width)
        {
            x = num;
            y++;
        }
        if (y == height)
        {
            y = 0;
            num += 256;
            x = num;
        }
    }

    return img;
}

QImage ImageConverter::convertBARG4444(QByteArray &array, int width, int height)
///BARG = RGBA (endianness)
{
    QImage img(width, height, QImage::Format_ARGB32);

    img.fill(Qt::transparent);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uchar ba = array.at(y * 2 * height + x * 2);
            uchar rg = array.at(y * 2 * height + x * 2 + 1);

            uchar b = ba >> 4;
            uchar a = ba & 0xF;
            uchar r = rg >> 4;
            uchar g = rg & 0xF;

            img.setPixel(x, y, qRgba(r * 0x11, g * 0x11, b * 0x11, a * 0x11));
        }
    }

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
    case ARGB555:
        return convertARGB555(array, width, height);
        break;
    case GBAR4444:
        return convertGBAR4444(array, width, height);
        break;
    case NSTC:
        return convertNSTC(array, width, height);
        break;
    case BGRA8888_INTERLACED:
        return convertBGRA8888_INTERLACED(array, width, height);
        break;
    case BARG4444:
        return convertBARG4444(array, width, height);
        break;

    default:
        break;
    }
}
