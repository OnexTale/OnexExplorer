#include "OnexNStcData.h"

OnexNStcData::OnexNStcData(QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
        : OnexTreeImage(name, content, opener, id, creationDate, compressed) {
    if (content.isEmpty())
        this->content = QByteArrayLiteral("\x00\x00\x00\x00\x00\x00\x00\x00");
}

OnexNStcData::~OnexNStcData() = default;

QImage OnexNStcData::getImage() {
    ImageResolution resolution = this->getResolution();
    return imageConverter->convertNSTC(content, resolution.x, resolution.y, 4);
}

ImageResolution OnexNStcData::getResolution() {
    int x = opener->getLittleEndianConverter()->fromShort(content.mid(0, 2));
    int y = opener->getLittleEndianConverter()->fromShort(content.mid(2, 2));
    return ImageResolution{x, y};
}

int OnexNStcData::afterReplace(QImage image) {
    QByteArray newContent;
    newContent.push_back(opener->getLittleEndianConverter()->toShort(image.width()));
    newContent.push_back(opener->getLittleEndianConverter()->toShort(image.height()));
    newContent.push_back(imageConverter->toNSTC(image));
    setContent(newContent);
    setWidth(image.width(), true);
    setHeight(image.height(), true);

    emit OnexTreeImage::replaceSignal(this->getImage());
    emit replaceInfo(generateInfos());
    return 1;
}

void OnexNStcData::setWidth(int width, bool update) {
    content.replace(0, 2, opener->getLittleEndianConverter()->toShort(width));
    if (update)
            emit changeSignal("Width", width);
}

void OnexNStcData::setHeight(int height, bool update) {
    content.replace(2, 2, opener->getLittleEndianConverter()->toShort(height));
    if (update)
            emit changeSignal("Width", height);
}

bool OnexNStcData::hasGoodResolution(int x, int y) {
    ImageResolution currentResolution = this->getResolution();
    return (x / 2 == currentResolution.x && y / 2 == currentResolution.y);
}

