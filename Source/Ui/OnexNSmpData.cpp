#include "OnexNSmpData.h"

OnexNSmpData::OnexNSmpData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeImage(name, content, opener, id, creationDate, compressed)
{

}

QImage OnexNSmpData::getImage()
{
    return QImage();
}

ImageResolution OnexNSmpData::getResolution()
{
    return ImageResolution();
}

OnexNSmpData::~OnexNSmpData()
{

}
