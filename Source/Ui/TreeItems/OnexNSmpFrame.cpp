#include "OnexNSmpFrame.h"

OnexNSmpFrame::OnexNSmpFrame(QString name, QByteArray content, int width, int height, int xOrigin, int yOrigin, NosZlibOpener *opener,
                             int id, int creationDate, bool compressed)
        : OnexTreeImage(name, content, opener, id, creationDate, compressed) {
    setFlags(this->flags() & (~Qt::ItemIsEditable));
    this->width = width;
    this->height = height;
    this->xOrigin = xOrigin;
    this->yOrigin = yOrigin;
}

OnexNSmpFrame::~OnexNSmpFrame() = default;

FileInfo *OnexNSmpFrame::getInfos() {
    FileInfo *infos = generateInfos();
    connect(this, SIGNAL(replaceInfo(FileInfo * )), infos, SLOT(replace(FileInfo * )));
    return infos;
}

QImage OnexNSmpFrame::getImage() {
    ImageResolution resolution = this->getResolution();
    return imageConverter->convertGBAR4444(content, resolution.x, resolution.y);
}

ImageResolution OnexNSmpFrame::getResolution() {
    return ImageResolution{this->width, this->height};
}

int OnexNSmpFrame::afterReplace(QImage image) {
    setWidth(image.width(), true);
    setHeight(image.height(), true);
    setContent(imageConverter->toGBAR4444(image));
    emit OnexTreeImage::replaceSignal(this->getImage());
    FileInfo *newInfo = generateInfos();
    emit replaceInfo(newInfo);
    emit replaceInfo(generateInfos());
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

void OnexNSmpFrame::setId(int id, bool update) {
    this->id = id;
}

void OnexNSmpFrame::setWidth(int width, bool update) {
    this->width = width;
    if (update)
            emit changeSignal("Width", width);
}

void OnexNSmpFrame::setHeight(int height, bool update) {
    this->height = height;
    if (update)
            emit changeSignal("Height", height);
}

void OnexNSmpFrame::setXOrigin(int xOrigin, bool update) {
    this->xOrigin = xOrigin;
    if (update)
            emit changeSignal("x-Origin", xOrigin);
}

void OnexNSmpFrame::setYOrigin(int yOrigin, bool update) {
    this->yOrigin = yOrigin;
    if (update)
            emit changeSignal("y-Origin", yOrigin);
}

FileInfo *OnexNSmpFrame::generateInfos() {
    FileInfo *infos = OnexTreeItem::generateInfos();
    infos->addIntLineEdit("Width", getWidth())->setDisabled(true);
    infos->addIntLineEdit("Height", getHeight())->setDisabled(true);
    connect(infos->addIntLineEdit("x-Origin", getXOrigin()), &QLineEdit::textChanged,
            [=](const QString &value) { setXOrigin(value.toInt()); });
    connect(infos->addIntLineEdit("y-Origin", getYOrigin()), &QLineEdit::textChanged,
            [=](const QString &value) { setYOrigin(value.toInt()); });
    return infos;
}
