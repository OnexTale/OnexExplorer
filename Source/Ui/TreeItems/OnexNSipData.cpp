#include "OnexNSipData.h"

OnexNSipData::OnexNSipData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
        : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {
}

OnexNSipData::~OnexNSipData() = default;

QImage OnexNSipData::getImage() {
    ImageResolution resolution = this->getResolution();
    return imageConverter->convertGBAR4444(content, resolution.x, resolution.y, 13);
}

ImageResolution OnexNSipData::getResolution() {
    int x = opener->getLittleEndianConverter()->fromShort(content.mid(1, 2));
    int y = opener->getLittleEndianConverter()->fromShort(content.mid(3, 2));
    return ImageResolution{x, y};
}

ImageResolution OnexNSipData::getCenter() {
    int x = opener->getLittleEndianConverter()->fromShort(content.mid(5, 2));
    int y = opener->getLittleEndianConverter()->fromShort(content.mid(7, 2));
    return ImageResolution{x, y};
}

int OnexNSipData::afterReplace(QImage image) {
    QByteArray newContent;
    newContent.push_back(content.mid(0, 1));
    newContent.push_back(opener->getLittleEndianConverter()->toShort(image.width()));
    newContent.push_back(opener->getLittleEndianConverter()->toShort(image.height()));
    newContent.push_back(content.mid(5, 8));
    newContent.push_back(imageConverter->toGBAR4444(image));
    setContent(newContent);
    setWidth(image.width(), true);
    setHeight(image.height(), true);

    emit OnexTreeImage::replaceSignal(this->getImage());
    return 1;
}

void OnexNSipData::setWidth(int width, bool update) {
    content.replace(1, 2, opener->getLittleEndianConverter()->toShort(width));
    if (update)
            emit changeSignal("Width", width);
}

void OnexNSipData::setHeight(int height, bool update) {
    content.replace(3, 2, opener->getLittleEndianConverter()->toShort(height));
    if (update)
            emit changeSignal("Height", height);
}

void OnexNSipData::setCenterX(int center, bool update) {
    content.replace(5, 2, opener->getLittleEndianConverter()->toShort(center));
    if (update)
            emit changeSignal("Center-X", center);
}

void OnexNSipData::setCenterY(int center, bool update) {
    content.replace(7, 2, opener->getLittleEndianConverter()->toShort(center));
    if (update)
            emit changeSignal("Center-Y", center);
}

FileInfo *OnexNSipData::generateInfos() {
    FileInfo *infos = OnexTreeImage::generateInfos();
    connect(infos->addIntLineEdit("Center-X", getCenter().x), &QLineEdit::textChanged,
            [=](const QString &value) { setCenterY(value.toInt()); });
    connect(infos->addIntLineEdit("Center-Y", getCenter().y), &QLineEdit::textChanged,
            [=](const QString &value) { setCenterX(value.toInt()); });
    return infos;
}