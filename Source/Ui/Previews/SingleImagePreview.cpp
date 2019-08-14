#include "SingleImagePreview.h"
#include "ui_SingleImagePreview.h"

SingleImagePreview::SingleImagePreview(const QImage &image, QWidget *parent)
        : QWidget(parent), ui(new Ui::SingleImagePreview) {
    ui->setupUi(this);
    ui->imgContent->setPixmap(QPixmap::fromImage(image));
}

SingleImagePreview::~SingleImagePreview() {
    delete ui;
}

void SingleImagePreview::onReplaced(const QImage &newImage) {
    ui->imgContent->setPixmap(QPixmap::fromImage(newImage));
}
