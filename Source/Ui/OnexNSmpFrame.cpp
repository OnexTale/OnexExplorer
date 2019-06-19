#include "OnexNSmpFrame.h"

OnexNSmpFrame::OnexNSmpFrame(QString name, QByteArray content, int width, int height, int xOrigin, int yOrigin,
                             NosZlibOpener *opener, int id, int creationDate, bool compressed)
    : OnexTreeImage(name, content, opener, id, creationDate, compressed) {
    this->width = width;
    this->height = height;
    this->xOrigin = xOrigin;
    this->yOrigin = yOrigin;
}

QImage OnexNSmpFrame::getImage() {
    ImageResolution resolution = this->getResolution();
    return opener->getImageConverter().convertGBAR4444(content, resolution.x, resolution.y);
}

ImageResolution OnexNSmpFrame::getResolution() { return ImageResolution{this->width, this->height}; }

int OnexNSmpFrame::onReplace(QString directory) {
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

    content = opener->getImageConverter().toGBAR4444(image);
    emit OnexTreeImage::replaceSignal(this->getImage());

    return 1;
}

int OnexNSmpFrame::getWidth() { return width; }
int OnexNSmpFrame::getHeight() { return height; }
int OnexNSmpFrame::getXOrigin() { return xOrigin; }
int OnexNSmpFrame::getYOrigin() { return yOrigin; }
QByteArray OnexNSmpFrame::getContent() { return content; }

OnexNSmpFrame::~OnexNSmpFrame() {}
