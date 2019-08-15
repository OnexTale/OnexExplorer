#include "OnexNStpData.h"
#include "OnexNStpMipMap.h"

OnexNStpData::OnexNStpData(const QByteArray &header, const QString &name, const QByteArray &content, NosZlibOpener *opener,

                           int id, int creationDate, bool compressed)
        : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {
    if (id == -1)
        return;
    generateMipMap();
}

OnexNStpData::~OnexNStpData() = default;

QByteArray OnexNStpData::getContent() {
    int amount = childCount();
    if (!hasParent() || amount <= 0)
        return content;
    QByteArray contentArray = content.mid(0, 8);
    for (int i = 0; i < amount; i++) {
        auto *currentItem = dynamic_cast<OnexNStpMipMap *>(this->child(i));
        contentArray.push_back(currentItem->getContent());
    }
    this->content = contentArray;
    return content;
}

QImage OnexNStpData::getImage() {
    if (childCount() > 0)
        return dynamic_cast<OnexNStpMipMap *>(this->child(0))->getImage();
    ImageResolution resolution = this->getResolution();
    int format = this->getFormat();
    if (format == 0)
        return imageConverter->convertGBAR4444(content, resolution.x, resolution.y, 8);
    else if (format == 1)
        return imageConverter->convertARGB555(content, resolution.x, resolution.y, 8);
    else if (format == 2)
        return imageConverter->convertBGRA8888(content, resolution.x, resolution.y, 8);
    else if (format == 3 || format == 4)
        return imageConverter->convertGrayscale(content, resolution.x, resolution.y, 8);
    else {
        qDebug().noquote().nospace() << "Unknown format! (" << format << ")";
        return QImage(resolution.x, resolution.y, QImage::Format_Invalid);
    }
}

ImageResolution OnexNStpData::getResolution() {
    int x = opener->getLittleEndianConverter()->fromShort(content.mid(0, 2));
    int y = opener->getLittleEndianConverter()->fromShort(content.mid(2, 2));
    return ImageResolution{x, y};
}

int OnexNStpData::getFormat() {
    return content.at(4);
}

int OnexNStpData::getFileAmount() {
    return content.at(7);
}

int OnexNStpData::afterReplace(QImage image) {
    int format = this->getFormat();
    if (format < 0 || format > 4) {
        QMessageBox::critical(nullptr, "Woops", "Format of " + name + "is not supported!");
        return 0;
    }
    QByteArray newContent;
    newContent.push_back(opener->getLittleEndianConverter()->toShort(image.width()));
    newContent.push_back(opener->getLittleEndianConverter()->toShort(image.height()));
    newContent.push_back(content.mid(4, 4));
    if (format == 0)
        newContent.push_back(imageConverter->toGBAR4444(image));
    else if (format == 1)
        newContent.push_back(imageConverter->toARGB555(image));
    else if (format == 2)
        newContent.push_back(imageConverter->toBGRA8888(image));
    else if (format == 3 || format == 4)
        newContent.push_back(imageConverter->toGrayscale(image));
    setContent(newContent);
    setWidth(image.width(), true);
    setHeight(image.height(), true);

    emit OnexTreeImage::replaceSignal(this->getImage());
    return 1;
}

void OnexNStpData::setWidth(int width, bool update) {
    content.replace(0, 2, opener->getLittleEndianConverter()->toShort(width));
    if (update)
            emit changeSignal("Width", width);
}

void OnexNStpData::setHeight(int height, bool update) {
    content.replace(2, 2, opener->getLittleEndianConverter()->toShort(height));
    if (update)
            emit changeSignal("Height", height);
}

void OnexNStpData::setFormat(uint8_t format, bool update) {
    content[4] = format;
    if (update)
            emit changeSignal("Format", format);
}

FileInfo *OnexNStpData::generateInfos() {
    FileInfo *infos = OnexTreeImage::generateInfos();
    connect(infos->addIntLineEdit("Format", getFormat()), &QLineEdit::textChanged,
            [=](const QString &value) { setFormat(value.toInt()); });
    return infos;
}

void OnexNStpData::generateMipMap() {
    int amount = getFileAmount();
    ImageResolution res = getResolution();
    int format = getFormat();
    int offset = 8;
    for (int i = 0; i < amount; i++) {
        int nextOffset = offset;
        if (format == 0 || format == 1)
            nextOffset += res.x * res.y * 2;
        else if (format == 2)
            nextOffset += res.x * res.y * 4;
        else
            nextOffset += res.x * res.y;
        this->addChild(new OnexNStpMipMap(header, name + "_" + QString::number(res.x) + "x" + QString::number(res.y),
                                          content.mid(offset, nextOffset - offset), res.x, res.y, format, (NosZlibOpener *) opener, id,
                                          creationDate, compressed));
        offset = nextOffset;
        res.x /= 2;
        res.y /= 2;
    }
}