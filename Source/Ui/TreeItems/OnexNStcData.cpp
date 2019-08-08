#include "OnexNStcData.h"

OnexNStcData::OnexNStcData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
    : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {
}

QImage OnexNStcData::getImage() {
    ImageResolution resolution = this->getResolution();

    return imageConverter.convertNSTC(content, resolution.x, resolution.y, 4);
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

        image = image.scaled(image.width() / 2, image.height() / 2);

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
        newContent.push_back(content.mid(4, 4));
        newContent.push_back(imageConverter.toNSTC(image));

        content = newContent;
        setWidth(image.width());
        setHeight(image.height());

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
    }
}

void OnexNStcData::setWidth(int width, bool update) {
    content.replace(0, 2, fromShortToLittleEndian(width));
    if (update)
        emit changeSignal("Width", width);
}
void OnexNStcData::setHeight(int height, bool update) {
    content.replace(2, 2, fromShortToLittleEndian(height));
    if (update)
        emit changeSignal("Width", height);
}

OnexNStcData::~OnexNStcData() {
}
