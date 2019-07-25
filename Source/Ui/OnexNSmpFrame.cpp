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
    return opener->getImageConverter().convertGBAR4444(content, resolution.x, resolution.y);
}

ImageResolution OnexNSmpFrame::getResolution() {
    return ImageResolution{this->width, this->height};
}

QWidget *OnexNSmpFrame::getInfos() {
    QWidget *w = OnexTreeImage::getInfos();
    QGridLayout *infoLayout = static_cast<QGridLayout *>(w->layout());

    QLabel *xLabel = new QLabel("x-Origin");
    infoLayout->addWidget(xLabel, 7, 0);
    QLineEdit *xIn = new QLineEdit(QString::number(getXOrigin()));
    connect(xIn, SIGNAL(textChanged(QString)), this, SLOT(setXOrigin(QString)));
    infoLayout->addWidget(xIn, 7, 1);

    QLabel *yLabel = new QLabel("y-Origin");
    infoLayout->addWidget(yLabel, 8, 0);
    QLineEdit *yIn = new QLineEdit(QString::number(getYOrigin()));
    connect(yIn, SIGNAL(textChanged(QString)), this, SLOT(setYOrigin(QString)));
    infoLayout->addWidget(yIn, 8, 1);

    return w;
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

    if (!hasGoodResolution(image.width(), image.height()))
        return 0;

    content = opener->getImageConverter().toGBAR4444(image);
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
}
void OnexNSmpFrame::setHeight(int height) {
    this->height = height;
}

void OnexNSmpFrame::setXOrigin(int xOrigin) {
    this->xOrigin = xOrigin;
}

void OnexNSmpFrame::setYOrigin(int yOrigin) {
    this->yOrigin = yOrigin;
}

void OnexNSmpFrame::setXOrigin(QString xOrigin) {
    setXOrigin(xOrigin.toInt());
}

void OnexNSmpFrame::setYOrigin(QString yOrigin) {
    setYOrigin(yOrigin.toInt());
}

OnexNSmpFrame::~OnexNSmpFrame() {
}
