#include "OnexNSipData.h"

OnexNSipData::OnexNSipData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeImage(name, content, opener, id, creationDate, compressed) {}

QImage OnexNSipData::getImage() {
    ImageResolution resolution = this->getResolution();

    return opener->getImageConverter().convertGBAR4444(content, resolution.x, resolution.y, 13);
}

ImageResolution OnexNSipData::getResolution() {
    int x = fromLittleEndianToShort(content.mid(1, 2));
    int y = fromLittleEndianToShort(content.mid(3, 2));

    return ImageResolution{x, y};
}

int OnexNSipData::onReplace(QString directory) {
    if (this->childCount() > 0) {
        int count = 0;
        for (int i = 0; i < this->childCount(); i++) {
            OnexNSipData *item = static_cast<OnexNSipData *>(this->child(i));
            QString path = directory + item->getName() + ".png";
            if (!QFile(path).exists()) {
                QMessageBox::critical(NULL, "Woops", "Missing " + path);
                continue;
            }

            QImage image = importQImageFromSelectedUserFile(path);
            if (image.isNull() && item->getResolution().x != 0 && item->getResolution().y != 0)
                continue;

            if (!hasGoodResolution(image.width(), image.height()))
                continue;

            QByteArray newContent;
            newContent.push_back(item->getContent().mid(0, 13));
            newContent.push_back(opener->getImageConverter().toGBAR4444(image));

            item->content = newContent;

            emit OnexTreeImage::replaceSignal(item->getImage());

            count++;
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

        QByteArray newContent;
        newContent.push_back(content.mid(0, 13));
        newContent.push_back(opener->getImageConverter().toGBAR4444(image));

        content = newContent;

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

OnexNSipData::~OnexNSipData() {}
