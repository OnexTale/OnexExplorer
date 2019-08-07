#include "OnexNS4BbData.h"

OnexNS4BbData::OnexNS4BbData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                             int creationDate, bool compressed)
    : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {
}

QImage OnexNS4BbData::getImage() {
    ImageResolution resolution = this->getResolution();
    setWidth(0);
    return imageConverter.convertBGRA8888_INTERLACED(content, resolution.x, resolution.y, 4);
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

        QByteArray newContent;
        newContent.push_back(fromShortToLittleEndian(image.width()));
        newContent.push_back(fromShortToLittleEndian(image.height()));
        newContent.push_back(imageConverter.toBGRA8888_INTERLACED(image));

        content = newContent;
        setWidth(image.width());
        setHeight(image.height());

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

void OnexNS4BbData::setWidth(int width) {
    content.replace(0, 2, fromShortToLittleEndian(width));
    emit changeSignal("Width", width);
}
void OnexNS4BbData::setHeight(int height) {
    content.replace(2, 2, fromShortToLittleEndian(height));
    emit changeSignal("Height", height);
}

OnexNS4BbData::~OnexNS4BbData() {
}
