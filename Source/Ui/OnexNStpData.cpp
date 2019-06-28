#include "OnexNStpData.h"
#include "OnexNStpMipMap.h"

OnexNStpData::OnexNStpData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeImage(name, content, opener, id, creationDate, compressed) {
    if (id == -1)
        return;
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
            return;

        this->addChild(new OnexNStpMipMap(name + "_" + QString::number(res.x) + "x" + QString::number(res.y),
                                          content.mid(offset, nextOffset), res.x, res.y, format, opener, id,
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

QImage OnexNStpData::getImage() {
    ImageResolution resolution = this->getResolution();

    int format = this->getFormat();

    if (format == 0)
        return opener->getImageConverter().convertGBAR4444(content, resolution.x, resolution.y, 8);
    else if (format == 1)
        return opener->getImageConverter().convertARGB555(content, resolution.x, resolution.y, 8);
    else if (format == 2)
        return opener->getImageConverter().convertBGRA8888(content, resolution.x, resolution.y, 8);
    // else if (format == 3 || format == 4)
    //     return opener->getImageConverter().convertNSTC(content, resolution.x, resolution.y, 8);
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

int OnexNStpData::onReplace(QString directory) {
    if (this->childCount() > 0) {
        int count = 0;
        for (int i = 0; i < this->childCount(); i++) {
            OnexNStpData *item = static_cast<OnexNStpData *>(this->child(i));
            count += item->onReplace(directory);
        }
        return count;
    } else {

        QString path = directory + this->getName() + ".png";
        if (!QFile(path).exists()) {
            QMessageBox::critical(NULL, "Woops", "Missing " + path);
            return 0;
        }

        QImage image = importQImageFromSelectedUserFile(path);
        if (image.isNull() && this->getResolution().x != 0 && this->getResolution().y != 0)
            return 0;

        if (!hasGoodResolution(image.width(), image.height()))
            return 0;

        int format = this->getFormat();

        if (format < 0 || format > 2)
            return 0;

        QByteArray newContent;
        newContent.push_back(content.mid(0, 8));
        if (format == 0)
            newContent.push_back(opener->getImageConverter().toGBAR4444(image));
        else if (format == 1)
            newContent.push_back(opener->getImageConverter().toARGB555(image));
        else if (format == 2)
            newContent.push_back(opener->getImageConverter().toBGRA8888(image));

        content = newContent;

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

int OnexNStpData::onExportAll(QString directory) {
    int count = 0;
    if (childCount() > 0) {
        for (int i = 0; i != this->childCount(); ++i) {
            OnexNStpData *item = static_cast<OnexNStpData *>(this->child(i));
            count += item->onExportAll(directory);
        }
    } else
        count = onExportSingle(directory);

    return count;
}

int OnexNStpData::onExportSingle(QString directory) {
    if (childCount() > 0)
        return onExportAll(directory);
    if (this->getImage().save(directory + getName() + ".png", "PNG", 100))
        return 1;
    return 0;
}

OnexNStpData::~OnexNStpData() {
}
