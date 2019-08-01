#include "OnexNStpMipMap.h"
#include "OnexNStpData.h"

OnexNStpMipMap::OnexNStpMipMap(QString name, QByteArray content, int width, int height, int format,
                               NosZlibOpener *opener, int id, int creationDate, bool compressed)
    : OnexTreeImage(name, content, opener, id, creationDate, compressed) {
    this->width = width;
    this->height = height;
    this->format = format;
}

QImage OnexNStpMipMap::getImage() {
    ImageResolution resolution = this->getResolution();
    int format = this->getFormat();

    if (format == 0)
        return opener->getImageConverter().convertGBAR4444(content, resolution.x, resolution.y);
    else if (format == 1)
        return opener->getImageConverter().convertARGB555(content, resolution.x, resolution.y);
    else if (format == 2)
        return opener->getImageConverter().convertBGRA8888(content, resolution.x, resolution.y);
     else if (format == 3 || format == 4)
         return opener->getImageConverter().convertGrayscale(content, resolution.x, resolution.y);
    else {
        qDebug().noquote().nospace() << "Unknown format! (" << format << ")";
        return QImage(resolution.x, resolution.y, QImage::Format_Invalid);
    }
}

ImageResolution OnexNStpMipMap::getResolution() {
    return ImageResolution{this->width, this->height};
}

int OnexNStpMipMap::onReplace(QString directory) {
        QString path = directory + this->getName() + ".png";
        if (!QFile(path).exists()) {
            OnexNStpData *pItem = static_cast<OnexNStpData*>(QTreeWidgetItem::parent());
            path = directory + pItem->getName() + ".png";
        }
        if (!QFile(path).exists()) {
            QMessageBox::critical(NULL, "Woops", "Missing " + path);
            return 0;
        }

        QImage image = importQImageFromSelectedUserFile(path);
        image = image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if (image.isNull() && this->getResolution().x != 0 && this->getResolution().y != 0)
            return 0;

        if (!hasGoodResolution(image.width(), image.height()))
            return 0;

        int format = this->getFormat();

        if (format < 0 || format > 2)
            return 0;

        QByteArray newContent;
        if (format == 0)
            newContent.push_back(opener->getImageConverter().toGBAR4444(image));
        else if (format == 1)
            newContent.push_back(opener->getImageConverter().toARGB555(image));
        else if (format == 2)
            newContent.push_back(opener->getImageConverter().toBGRA8888(image));
        else if (format == 3 || format == 4)
            newContent.push_back(opener->getImageConverter().toGrayscale(image));

        content = newContent;

        emit OnexTreeImage::replaceSignal(this->getImage());

        return 1;
}

int OnexNStpMipMap::getWidth() {
    return width;
}
int OnexNStpMipMap::getHeight() {
    return height;
}
int OnexNStpMipMap::getFormat() {
    return format;
}
QByteArray OnexNStpMipMap::getContent() {
    return content;
}

OnexNStpMipMap::~OnexNStpMipMap() {
}
