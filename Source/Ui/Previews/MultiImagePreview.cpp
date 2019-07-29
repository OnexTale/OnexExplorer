#include "MultiImagePreview.h"
#include "ui_MultiImagePreview.h"
#include <QPainter>

MultiImagePreview::MultiImagePreview(QList<QImage> *images, QWidget *parent)
    : QWidget(parent), ui(new Ui::MultiImagePreview) {
    ui->setupUi(this);
    this->images = images;
    QImage image = generateImage();
    ui->imgContent->setPixmap(QPixmap::fromImage(image));
}

MultiImagePreview::~MultiImagePreview() {
    delete ui;
}

QPair<int,int> MultiImagePreview::getResolution() {
    QPair<int,int> r;
    for (int i = 0; i < this->images->size(); ++i) {
        QImage img = this->images->at(i);
        if (img.width() > r.first)
            r.first = img.width();
        r.second += img.height() + 20;
    }
    return r;
}

QImage MultiImagePreview::generateImage() {
    QPair<int,int> res = this->getResolution();
    QImage image = QImage(res.first, res.second, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    int offset = 0;
    for (int i = 0; i < this->images->size(); ++i) {
        QImage img = this->images->at(i);
        QPainter painter(&image);
        painter.drawImage(QRect(0, offset, img.width(), img.height()), img,
                          QRect(0, 0, img.width(), img.height()));
        offset += img.height() + 20;
    }
    return image;
}
