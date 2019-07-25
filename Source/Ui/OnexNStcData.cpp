#include "OnexNStcData.h"

OnexNStcData::OnexNStcData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {}

QImage OnexNStcData::getImage() {
    ImageResolution resolution = this->getResolution();

    return opener->getImageConverter().convertNSTC(content, resolution.x, resolution.y, 4);
}

ImageResolution OnexNStcData::getResolution() {
    int x = fromLittleEndianToShort(content.mid(0, 2));
    int y = fromLittleEndianToShort(content.mid(2, 2));

    return ImageResolution{x, y};
}

int OnexNStcData::onReplace(QString directory) {
    if (this->childCount() > 0) {
        int count = 0;
        for (int i = 0; i < this->childCount(); i++) {
            OnexNStcData *item = static_cast<OnexNStcData *>(this->child(i));
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

        image = image.scaled(image.width() / 2, image.height() / 2);

        if (!hasGoodResolution(image.width(), image.height())) {
            qDebug() << "NStc wrong resolution";
            return 0;
        }

        QByteArray newContent;
        newContent.push_back(content.mid(0, 4));
        newContent.push_back(opener->getImageConverter().toNSTC(image));

        content = newContent;

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

void OnexNStcData::setWidth(int width) {
    content.replace(0, 2, fromShortToLittleEndian(width));
}
void OnexNStcData::setHeight(int height) {
    content.replace(2, 2, fromShortToLittleEndian(height));
}

OnexNStcData::~OnexNStcData() {}
