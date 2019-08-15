#include "OnexNStpMipMap.h"
#include "OnexNStpData.h"

OnexNStpMipMap::OnexNStpMipMap(QByteArray header, QString name, QByteArray content, int width, int height, int format, NosZlibOpener *opener,
                               int id, int creationDate, bool compressed)
        : OnexTreeImage(header, name, content, opener, id, creationDate, compressed), width(width), height(height),
          format(format) {
}

OnexNStpMipMap::~OnexNStpMipMap() = default;

QImage OnexNStpMipMap::getImage() {
    ImageResolution resolution = this->getResolution();
    int format = this->getFormat();
    if (format == 0)
        return imageConverter->convertGBAR4444(content, resolution.x, resolution.y);
    else if (format == 1)
        return imageConverter->convertARGB555(content, resolution.x, resolution.y);
    else if (format == 2)
        return imageConverter->convertBGRA8888(content, resolution.x, resolution.y);
    else if (format == 3 || format == 4)
        return imageConverter->convertGrayscale(content, resolution.x, resolution.y);
    else {
        qDebug().noquote().nospace() << "Unknown format! (" << format << ")";
        return QImage(resolution.x, resolution.y, QImage::Format_Invalid);
    }
}

ImageResolution OnexNStpMipMap::getResolution() {
    return ImageResolution{this->width, this->height};
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

int OnexNStpMipMap::afterReplace(QImage image) {
    int format = this->getFormat();
    if (format < 0 || format > 4) {
        QMessageBox::critical(nullptr, "Woops", "Format of " + name + "is not supported!");
        return 0;
    }
    QByteArray newContent;
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

    emit OnexTreeImage::replaceSignal(this->getImage());
    return 1;
}

void OnexNStpMipMap::setWidth(int width, bool update) {
    this->width = width;
    if (update)
            emit changeSignal("Width", width);
}

void OnexNStpMipMap::setHeight(int height, bool update) {
    this->height = height;
    if (update)
            emit changeSignal("Height", height);
}

void OnexNStpMipMap::setFormat(uint8_t format, bool update) {
    this->format = format;
    if (update)
            emit changeSignal("Format", format);
}

FileInfo *OnexNStpMipMap::generateInfos() {
    FileInfo *infos = OnexTreeImage::generateInfos();
    connect(infos->addIntLineEdit("Format", getFormat()), &QLineEdit::textChanged,
            [=](const QString &value) { setFormat(value.toInt()); });
    return infos;
}

