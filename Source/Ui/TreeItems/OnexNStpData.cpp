#include "OnexNStpData.h"
#include "OnexNStpMipMap.h"

OnexNStpData::OnexNStpData(const QString &name, QByteArray content, NosZlibOpener *opener,

                           int id, int creationDate, bool compressed)
        : OnexTreeImage(name, opener, content, id, creationDate, compressed) {
    if (content.isEmpty())
        this->content = QByteArrayLiteral("\x00\x00\x00\x00\x00\x00\x00\x00");
    if (id == -1)
        return;
    if (getFileAmount() > 0)
        generateMipMap(true);
}

OnexNStpData::OnexNStpData(QJsonObject jo, NosZlibOpener *opener, const QString &directory) : OnexTreeImage(jo["ID"].toString(), opener) {
    this->content = QByteArrayLiteral("\x00\x00\x00\x00\x00\x00\x00\x00");
    setId(jo["ID"].toInt(), true);
    setCreationDate(jo["Date"].toString(), true);
    setCompressed(jo["isCompressed"].toBool(), true);
    setFormat(jo["Format"].toInt());
    setSmoothScaling(jo["SmoothScaling"].toBool(), true);
    setUnknownValue(jo["Unknown"].toBool(), true);
    bool mip = jo["MipMap"].toBool();

    if (mip) {
        int amount = 0;
        int x = getResolution().x;
        while (x >= 2) {
            x /= 2;
            amount++;
        }
        setFileAmount(amount);
    }

    if (!jo["path"].toString().isEmpty())
        onReplace(directory + jo["path"].toString());

    if (getFileAmount() > 0) {
        ImageResolution res = getResolution();
        int format = getFormat();
        for (auto &&child : jo["content"].toArray()) {
            OnexTreeItem *item = new OnexNStpMipMap(name + "_" + QString::number(res.x) + "x" + QString::number(res.y),
                                                    QByteArray(), res.x, res.y, format, (NosZlibOpener *) opener, id,
                                                    creationDate, compressed);
            this->addChild(item);
            item->onReplace(directory + child.toObject()["path"].toString());
            res.x /= 2;
            res.y /= 2;
        }
        if (childCount() == 0)
            generateMipMap(true);
    }

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

bool OnexNStpData::getSmoothScaling() {
    return content.at(5);
}

bool OnexNStpData::getUnknownValue() {
    return content.at(6);
}

int OnexNStpData::getFileAmount() {
    return content.at(7);
}

int OnexNStpData::afterReplace(QImage image) {
    if (image.isNull()) {
        emit OnexTreeImage::replaceSignal(this->getImage());
        return 0;
    }

    qDebug() << getFileAmount();
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
    qDebug() << getFileAmount();
    generateMipMap(getFileAmount() != 0);


    emit OnexTreeImage::replaceSignal(this->getImage());
    emit replaceInfo(generateInfos());
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
    for (int i = 0; i < childCount(); i++) {
        auto *item = static_cast<OnexNStpMipMap *>(child(i));
        item->setFormat(format, update);
    }
}

void OnexNStpData::setSmoothScaling(bool smooth, bool update) {
    content[5] = smooth;
    if (update)
            emit changeSignal("SmoothScaling", smooth);
}

void OnexNStpData::setUnknownValue(bool unkown, bool update) {
    content[6] = unkown;
    if (update)
            emit changeSignal("Unknown", unkown);
}

void OnexNStpData::setFileAmount(uint8_t value, bool update) {
    content[7] = value;
    if (update)
            emit changeSignal("MipMap", value != 0);
}

FileInfo *OnexNStpData::generateInfos() {
    FileInfo *infos = OnexTreeImage::generateInfos();
    if (hasParent()) {
        const QStringList formats = {"ARGB4444", "ARGB1555 ", "ARGB8888", "Grayscale_1", "Grayscale_2"};
        connect(infos->addSelection("Format", formats, getFormat()), QOverload<int>::of(&QComboBox::currentIndexChanged),
                [=](const int &value) { setFormat(value); });
        connect(infos->addCheckBox("SmoothScaling", getSmoothScaling()), &QCheckBox::clicked,
                [=](const bool value) { setSmoothScaling(value); });
        connect(infos->addCheckBox("Unknown", getUnknownValue()), &QCheckBox::clicked,
                [=](const bool value) { setUnknownValue(value); });
        connect(infos->addCheckBox("MipMap", getFileAmount() != 0), &QCheckBox::clicked,
                [=](const bool value) { generateMipMap(value); });
    }
    return infos;
}

void OnexNStpData::generateMipMap(bool generate) {
    this->takeChildren();

    if (!generate) {
        setFileAmount(0);
        return;
    }

    int amount = 0;
    int x = getResolution().x;
    while (x >= 2) {
        x /= 2;
        amount++;
    }

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

        if (nextOffset > content.size()) {
            QImage subImage = getImage();
            subImage = subImage.scaled(res.x, res.y, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QByteArray subContent;
            if (format == 0)
                subContent = imageConverter->toGBAR4444(subImage);
            else if (format == 1)
                subContent = imageConverter->toARGB555(subImage);
            else if (format == 2)
                subContent = imageConverter->toBGRA8888(subImage);
            else if (format == 3 || format == 4)
                subContent = imageConverter->toGrayscale(subImage);

            this->addChild(new OnexNStpMipMap(name + "_" + QString::number(res.x) + "x" + QString::number(res.y),
                                              subContent, res.x, res.y, format, (NosZlibOpener *) opener, id, creationDate, compressed));
        } else {
            this->addChild(new OnexNStpMipMap(name + "_" + QString::number(res.x) + "x" + QString::number(res.y),
                                              content.mid(offset, nextOffset - offset), res.x, res.y, format, (NosZlibOpener *) opener, id,
                                              creationDate, compressed));
        }
        offset = nextOffset;
        res.x /= 2;
        res.y /= 2;
    }

    setFileAmount(amount);
}

void OnexNStpData::setName(QString name) {
    OnexTreeZlibItem::setName(name);
    QList<QTreeWidgetItem *> childList = takeChildren();
    for (int i = 0; i < childList.size(); i++) {
        auto *item = static_cast<OnexNStpMipMap *>(childList.at(i));
        item->OnexTreeItem::setName(name + "_" + QString::number(item->getWidth()) + "x" + QString::number(item->getHeight()));
    }
    addChildren(childList);
}
