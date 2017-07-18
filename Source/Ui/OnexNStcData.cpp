#include "OnexNStcData.h"

OnexNStcData::OnexNStcData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeImage(name, content, opener, id, creationDate, compressed)
{

}

QImage OnexNStcData::getImage()
{
    ImageResolution resolution = this->getResolution();

    return opener->getImageConverter().convertNSTC(content, resolution.x, resolution.y, 4);
}

ImageResolution OnexNStcData::getResolution()
{
    int x = fromLittleEndianToShort(content.mid(0, 2));
    int y = fromLittleEndianToShort(content.mid(0, 2));

    return ImageResolution{x, y};
}

OnexNStcData::~OnexNStcData()
{

}
