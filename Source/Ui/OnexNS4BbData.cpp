#include "OnexNS4BbData.h"

OnexNS4BbData::OnexNS4BbData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                             bool compressed)
    : OnexTreeImage(name, content, opener, id, creationDate, compressed) {
}

QImage OnexNS4BbData::getImage() {
    ImageResolution resolution = this->getResolution();

    return opener->getImageConverter().convertBGRA8888_INTERLACED(content, resolution.x, resolution.y, 4);
}

ImageResolution OnexNS4BbData::getResolution() {
    int x = fromLittleEndianToShort(content.mid(0, 2));
    int y = fromLittleEndianToShort(content.mid(2, 2));

    return ImageResolution{x, y};
}

int OnexNS4BbData::onReplace(QString directory) {
    if (this->childCount() > 0) {
        int count = 0;
        for (int i = 0; i < this->childCount(); i++) {
            OnexNS4BbData *item = static_cast<OnexNS4BbData *>(this->child(i));
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

        QByteArray newContent;
        newContent.push_back(content.mid(0, 4));
        newContent.push_back(opener->getImageConverter().toBGRA8888_INTERLACED(image));

        content = newContent;

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

OnexNS4BbData::~OnexNS4BbData() {
}
