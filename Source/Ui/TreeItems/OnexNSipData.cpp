#include "OnexNSipData.h"

OnexNSipData::OnexNSipData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
    : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {
}

QImage OnexNSipData::getImage() {
    ImageResolution resolution = this->getResolution();

    return imageConverter.convertGBAR4444(content, resolution.x, resolution.y, 13);
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
            count += item->onReplace(directory);
        }
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

        QByteArray newContent;
        newContent.push_back(content.mid(0, 1));
        newContent.push_back(fromShortToLittleEndian(image.width()));
        newContent.push_back(fromShortToLittleEndian(image.height()));
        newContent.push_back(content.mid(5, 8));
        newContent.push_back(imageConverter.toGBAR4444(image));

        content = newContent;
        setWidth(image.width(),true);
        setHeight(image.height(),true);

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

OnexNSipData::~OnexNSipData() {
}

void OnexNSipData::setWidth(int width, bool update) {
    content.replace(1, 2, fromShortToLittleEndian(width));
    if (update)
        emit changeSignal("Width", width);
}
void OnexNSipData::setHeight(int height, bool update) {
    content.replace(3, 2, fromShortToLittleEndian(height));
    if (update)
        emit changeSignal("Height", height);
}
