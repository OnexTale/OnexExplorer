#include "OnexNSmpFrame.h"

OnexNSmpFrame::OnexNSmpFrame(QByteArray header, QString name, QByteArray content, int width, int height, int xOrigin,
                             int yOrigin, NosZlibOpener *opener, int id, int creationDate, bool compressed)
    : OnexTreeImage(header, name, content, opener, id, creationDate, compressed) {
    this->width = width;
    this->height = height;
    this->xOrigin = xOrigin;
    this->yOrigin = yOrigin;
}

QImage OnexNSmpFrame::getImage() {
    ImageResolution resolution = this->getResolution();
    return imageConverter.convertGBAR4444(content, resolution.x, resolution.y);
}

ImageResolution OnexNSmpFrame::getResolution() {
    return ImageResolution{this->width, this->height};
}

FileInfo *OnexNSmpFrame::getInfos() {
    FileInfo *infos = OnexTreeImage::getInfos();

    connect(infos->addIntLineEdit("x-Origin", getXOrigin()), &QLineEdit::textChanged,
            [=](const QString &value) { setXOrigin(value.toInt()); });

    connect(infos->addIntLineEdit("y-Origin", getYOrigin()), &QLineEdit::textChanged,
            [=](const QString &value) { setYOrigin(value.toInt()); });

    return infos;
}

int OnexNSmpFrame::onReplace(QString directory) {
    QString path = directory + this->getName() + ".png";
    if (!QFile(path).exists()) {
        QMessageBox::critical(NULL, "Woops", "Missing " + path);
        return 0;
    }

    QImage image = importQImageFromSelectedUserFile(path);
    if (image.isNull() && this->getResolution().x != 0 && this->getResolution().y != 0)
        return 0;

    if (!hasGoodResolution(image.width(), image.height())) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            0, "Resolution changed",
            "The resolution of the image " + name + " doesn't match!\nDo you want to replace it anyway?");
        if (reply == QMessageBox::No)
            return 0;
    }

    setWidth(image.width());
    setHeight(image.height());

    content = imageConverter.toGBAR4444(image);
    emit OnexTreeImage::replaceSignal(this->getImage());

    return 1;
}

int OnexNSmpFrame::getWidth() {
    return width;
}
int OnexNSmpFrame::getHeight() {
    return height;
}
int OnexNSmpFrame::getXOrigin() {
    return xOrigin;
}
int OnexNSmpFrame::getYOrigin() {
    return yOrigin;
}
QByteArray OnexNSmpFrame::getContent() {
    return content;
}

void OnexNSmpFrame::setWidth(int width) {
    this->width = width;
    emit changeSignal("Width", width);
}
void OnexNSmpFrame::setHeight(int height) {
    this->height = height;
    emit changeSignal("Height", height);
}

void OnexNSmpFrame::setXOrigin(int xOrigin) {
    this->xOrigin = xOrigin;
    emit changeSignal("x-Origin", xOrigin);
}

void OnexNSmpFrame::setYOrigin(int yOrigin) {
    this->yOrigin = yOrigin;
    emit changeSignal("y-Origin", yOrigin);
}

OnexNSmpFrame::~OnexNSmpFrame() {
}
