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

void OnexNSipData::onReplace()
{
    QString fileName = getOpenDirectory("PNG Image (*.png)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QImage image = QImage::fromData(file.readAll());
    file.close();

    if (!hasGoodResolution(image.width(), image.height()))
        return;

    QByteArray newContent;
    newContent.push_back(content.mid(0, 13));
    newContent.push_back(opener->getImageConverter().toGBAR4444(image));

    content = newContent;

    emit OnexTreeImage::replaceSignal(this->getImage());
}

OnexNSipData::~OnexNSipData()
{

}
