#include "OnexNS4BbData.h"

OnexNS4BbData::OnexNS4BbData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeImage(name, content, opener, id, creationDate, compressed)
{

}

QImage OnexNS4BbData::getImage()
{
    ImageResolution resolution = this->getResolution();

    return opener->getImageConverter().convertBGRA8888_INTERLACED(content, resolution.x, resolution.y, 4);
}

ImageResolution OnexNS4BbData::getResolution()
{
    int x = fromLittleEndianToShort(content.mid(0, 2));
    int y = fromLittleEndianToShort(content.mid(2, 2));

    return ImageResolution{x, y};
}

OnexNS4BbData::~OnexNS4BbData()
{

}
