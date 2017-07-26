#include "ImageConverter.h"

QImage ImageConverter::convertGBAR4444(QByteArray &array, int width, int height, int startByte)
///GBAR = ARGB (endianness)
{
    QImage img(width, height, QImage::Format_ARGB32);

    img.fill(Qt::transparent);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uchar gb = array.at(startByte + y * 2 * height + x * 2);
            uchar ar = array.at(startByte + y * 2 * height + x * 2 + 1);

            uchar g = gb >> 4;
            uchar b = gb & 0xF;
            uchar a = ar >> 4;
            uchar r = ar & 0xF;

            img.setPixel(x, y, qRgba(r * 0x11, g * 0x11, b * 0x11, a * 0x11));
        }
    }

    return img;
}

QImage ImageConverter::convertBGRA8888(QByteArray &array, int width, int height, int startByte)
{
    QImage img(width, height, QImage::Format_ARGB32);

    img.fill(Qt::transparent);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uchar b = array.at(startByte + y * 4 * height + x * 4);
            uchar g = array.at(startByte + y * 4 * height + x * 4 + 1);
            uchar r = array.at(startByte + y * 4 * height + x * 4 + 2);
            uchar a = array.at(startByte + y * 4 * height + x * 4 + 3);

            img.setPixel(x, y, qRgba(r, g, b, a));
        }
    }

    return img;
}

QImage ImageConverter::convertARGB555(QByteArray& array, int width, int height, int startByte)
{
    QImage img(width, height, QImage::Format_ARGB32);

    img.fill(Qt::transparent);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            ushort bytes = qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.mid(startByte + y * 2 * height + x * 2, 2).data()));

            uchar a = 255*(bytes & 0x8000) >> 15; // 0x8000 = 0b1000000000000000
            uchar r = 8*(bytes &   0x7C00) >> 10; // 0x7C00 = 0b0111110000000000
            uchar g = 8*(bytes &   0x3E0) >> 5; //0x3E0 = 0b0000001111100000
            uchar b = 8*(bytes &   0x1F); // 0x1F = 0b0000000000011111

            img.setPixel(x, y, qRgba(r, g, b, a));
        }
    }

    return img;
}

QImage ImageConverter::convertNSTC(QByteArray &array, int width, int height, int startByte)
{
    QImage img(width, height, QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            uchar value = array.at(startByte + y * width + x);
            if (colors.contains(value))
                img.setPixel(x, y, colors[value].rgb());
            else
                img.setPixel(x, y, colors[0xFF].rgb());
        }
    }

    return img.scaled(QSize(width*2, height*2));
}

QImage ImageConverter::convertBGRA8888_INTERLACED(QByteArray &array, int width, int height, int startByte)
{
    QImage img(width, height, QImage::Format_RGBA8888);
    img.fill(Qt::transparent);

    short num = 0,
            x = 0,
            y = 0;
    for (int i = 0; i < array.size() - startByte;)
    {
        uchar b = array.at(startByte + i++);
        uchar g = array.at(startByte + i++);
        uchar r = array.at(startByte + i++);
        uchar a = array.at(startByte + i++);
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

QImage ImageConverter::convertBARG4444(QByteArray &array, int width, int height, int startByte)
///BARG = RGBA (endianness)
{
    QImage img(width, height, QImage::Format_ARGB32);

    img.fill(Qt::transparent);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uchar ba = array.at(startByte + y * 2 * height + x * 2);
            uchar rg = array.at(startByte + y * 2 * height + x * 2 + 1);

            uchar b = ba >> 4;
            uchar a = ba & 0xF;
            uchar r = rg >> 4;
            uchar g = rg & 0xF;

            img.setPixel(x, y, qRgba(r * 0x11, g * 0x11, b * 0x11, a * 0x11));
        }
    }

    return img;
}

QByteArray ImageConverter::toGBAR4444(QImage &image)
{
    QByteArray data;

    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QRgb currentPixel = image.pixel(x, y);

            uchar green = qGreen(currentPixel) / 0x11;
            uchar blue = qBlue(currentPixel) / 0x11;
            uchar alpha = qAlpha(currentPixel) / 0x11;
            uchar red = qRed(currentPixel) / 0x11;

            uchar gb = (green << 4) + blue;
            uchar ar = (alpha << 4) + red;

            data.push_back(gb);
            data.push_back(ar);
        }
    }

    return data;
}

QByteArray ImageConverter::toNSTC(QImage &image)
{
    QByteArray data;
    int i = 0, j = 0;

    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            QRgb currentPixel = image.pixel(x, y);
            QColor pixColor = qRgb(qRed(currentPixel), qGreen(currentPixel), qBlue(currentPixel));
            if (colors.values().contains(pixColor))
            {
                i++;
                data.push_back(colors.key(pixColor));
            }
            else
            {
                j++;
                data.push_back(0xFF);
            }
        }
    }
    return data;
}

QByteArray ImageConverter::toBGRA8888(QImage &image)
{
    return QByteArray();
}

QByteArray ImageConverter::toARGB555(QImage &image)
{
    return QByteArray();
}

QByteArray ImageConverter::toBGRA8888_INTERLACED(QImage &image)
{
    return QByteArray();
}

QByteArray ImageConverter::toBARG4444(QImage &image)
{
    return QByteArray();
}

ImageConverter::ImageConverter()
{
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
    colors[0xFF] = qRgb(150, 0, 255);
}

