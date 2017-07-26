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
    int y = fromLittleEndianToShort(content.mid(2, 2));

    return ImageResolution{x, y};
}

void OnexNStcData::onReplace()
{
    QImage image = importQImageFromSelectedUserFile();
    if (image.isNull())
        return;

    image = image.scaled(image.width()/2, image.height()/2);

    if (!hasGoodResolution(image.width(), image.height()))
    {
        qDebug() << "NStc wrong resolution";
        return;
    }

    QByteArray newContent;
    newContent.push_back(content.mid(0, 4));
    newContent.push_back(opener->getImageConverter().toNSTC(image));

    content = newContent;

    emit OnexTreeImage::replaceSignal(this->getImage());
}

OnexNStcData::~OnexNStcData()
{

}
