#include "OnexNStpData.h"
#include "OnexNStpMipMap.h"

OnexNStpData::OnexNStpData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
    : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {
    if (id == -1)
        return;

    int amount = getFileAmount();
    ImageResolution res = getResolution();
    int format = getFormat();
    if (format > 2)
        return;
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
                                          content.mid(offset, nextOffset - offset), res.x, res.y, format, opener, id,
                                          creationDate, compressed));
        offset = nextOffset;
        res.x /= 2;
        res.y /= 2;
    }
}

int OnexNStpData::getFormat() {
    return content.at(4);
}

int OnexNStpData::getFileAmount() {
    return content.at(7);
}

QByteArray OnexNStpData::getContent() {
    int amount = childCount();
    if (!hasParent() || amount <= 0)
        return content;

    QByteArray contentArray = content.mid(0, 8);

    for (int i = 0; i < amount; i++) {
        OnexNStpMipMap *currentItem = static_cast<OnexNStpMipMap *>(this->child(i));
        contentArray.push_back(currentItem->getContent());
    }

    this->content = contentArray;
    return content;
}

QImage OnexNStpData::getImage() {
    if (childCount() > 0)
        return static_cast<OnexNStpData *>(this->child(0))->getImage();

    ImageResolution resolution = this->getResolution();

    int format = this->getFormat();

    if (format == 0)
        return imageConverter.convertGBAR4444(content, resolution.x, resolution.y, 8);
    else if (format == 1)
        return imageConverter.convertARGB555(content, resolution.x, resolution.y, 8);
    else if (format == 2)
        return imageConverter.convertBGRA8888(content, resolution.x, resolution.y, 8);
    else if (format == 3 || format == 4)
        return imageConverter.convertGrayscale(content, resolution.x, resolution.y, 8);
    else {
        qDebug().noquote().nospace() << "Unknown format! (" << format << ")";
        return QImage(resolution.x, resolution.y, QImage::Format_Invalid);
    }
}

ImageResolution OnexNStpData::getResolution() {
    int x = fromLittleEndianToShort(content.mid(0, 2));
    int y = fromLittleEndianToShort(content.mid(2, 2));

    return ImageResolution{x, y};
}

FileInfo *OnexNStpData::getInfos() {
    if (!hasParent())
        return nullptr;

    FileInfo *infos = OnexTreeImage::getInfos();

    connect(infos->addIntLineEdit("Format", getFormat()), &QLineEdit::textChanged,
            [=](const QString &value) { setFormat(value.toInt()); });

    return infos;
}

int OnexNStpData::onReplace(QString directory) {
    if (this->childCount() > 0) {
        int count = 0;
        for (int i = 0; i < this->childCount(); i++) {
            OnexNStpData *item = static_cast<OnexNStpData *>(this->child(i));
            count += item->onReplace(directory);
        }
        emit replaceSignal(getImage());
        return count;
    } else {
        QString path;
        if (!directory.endsWith(".png"))
            path = directory + this->getName() + ".png";
        else
            path = directory;

        if (!QFile(path).exists()) {
            QMessageBox::critical(NULL, "Woops", "Missing " + path);
            return 0;
        }

        QImage image = importQImageFromSelectedUserFile(path);
        if (image.isNull() && this->getResolution().x != 0 && this->getResolution().y != 0) {
            QMessageBox::critical(NULL, "Woops", "Couldn't read image " + path);
            return 0;
        }

        if (!hasGoodResolution(image.width(), image.height())) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                0, "Resolution changed",
                "The resolution of the image " + name + " doesn't match!\nDo you want to replace it anyway?");
            if (reply == QMessageBox::No)
                return 0;
        }

        int format = this->getFormat();

        if (format < 0 || format > 4) {
            QMessageBox::critical(NULL, "Woops", "Format of " + path + "is not supported!");
            return 0;
        }

        QByteArray newContent;
        newContent.push_back(fromShortToLittleEndian(image.width()));
        newContent.push_back(fromShortToLittleEndian(image.height()));
        newContent.push_back(content.mid(4, 4));
        if (format == 0)
            newContent.push_back(imageConverter.toGBAR4444(image));
        else if (format == 1)
            newContent.push_back(imageConverter.toARGB555(image));
        else if (format == 2)
            newContent.push_back(imageConverter.toBGRA8888(image));
        else if (format == 3 || format == 4)
            newContent.push_back(imageConverter.toGrayscale(image));

        content = newContent;
        setWidth(image.width());
        setHeight(image.height());

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

int OnexNStpData::onExport(QString directory) {
    if (childCount() > 0) {
        int count = 0;
        for (int i = 0; i != this->childCount(); ++i) {
            OnexTreeImage *item = static_cast<OnexTreeImage *>(this->child(i));
            count += item->onExport(directory);
        }
        return count;
    } else {
        QString path = "";
        if (!directory.endsWith(".png"))
            path = directory + this->name + ".png";
        else
            path = directory;

        if (this->getImage().save(path, "PNG", 100))
            return 1;
    }
    return 0;
}

void OnexNStpData::setWidth(int width) {
    content.replace(0, 2, fromShortToLittleEndian(width));
    emit changeSignal("Width", width);
}
void OnexNStpData::setHeight(int height) {
    content.replace(2, 2, fromShortToLittleEndian(height));
    emit changeSignal("Height", height);
}

void OnexNStpData::setFormat(uint8_t format) {
    content[4] = format;
    emit changeSignal("Format", format);
}

OnexNStpData::~OnexNStpData() {
}
