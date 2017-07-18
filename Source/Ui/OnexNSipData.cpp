#include "OnexNSipData.h"

OnexNSipData::OnexNSipData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeImage(name, content, opener, id, creationDate, compressed)
{

}

QImage OnexNSipData::getImage()
{
    ImageResolution resolution = this->getResolution();

    return opener->getImageConverter().convertGBAR4444(content, resolution.x, resolution.y, 13);
}

ImageResolution OnexNSipData::getResolution()
{
    int x = fromLittleEndianToShort(content.mid(1, 2));
    int y = fromLittleEndianToShort(content.mid(3, 2));

    return ImageResolution{x, y};
}

OnexNSipData::~OnexNSipData()
{

}
