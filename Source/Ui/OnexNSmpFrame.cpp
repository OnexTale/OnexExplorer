#include "OnexNSmpFrame.h"

OnexNSmpFrame::OnexNSmpFrame(QString name, QByteArray content, int width, int height, int xOrigin, int yOrigin, NosZlibOpener *opener, int id, int creationDate, bool compressed) : OnexTreeImage(name, content, opener, id, creationDate, compressed)
{
    this->width = width;
    this->height = height;
    this->xOrigin = xOrigin;
    this->yOrigin = yOrigin;
}

QImage OnexNSmpFrame::getImage()
{
    ImageResolution resolution = this->getResolution();
    return opener->getImageConverter().convertGBAR4444(content, resolution.x, resolution.y);
}

ImageResolution OnexNSmpFrame::getResolution()
{
    return ImageResolution{this->width, this->height};
}

void OnexNSmpFrame::onReplace()
{
    QImage image = importQImageFromSelectedUserFile();
    if (image.isNull())
        return;

    if (!hasGoodResolution(image.width(), image.height()))
        return;

    content = opener->getImageConverter().toGBAR4444(image);
    emit OnexTreeImage::replaceSignal(this->getImage());
}

int OnexNSmpFrame::getWidth()
{
    return width;
}
int OnexNSmpFrame::getHeight()
{
    return height;
}
int OnexNSmpFrame::getXOrigin()
{
    return xOrigin;
}
int OnexNSmpFrame::getYOrigin()
{
    return yOrigin;
}
QByteArray OnexNSmpFrame::getContent()
{
    return content;
}

OnexNSmpFrame::~OnexNSmpFrame()
{
}
