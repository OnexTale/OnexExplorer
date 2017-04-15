#include "SpritesImagePreview.h"
#include "ui_SpritesImagePreview.h"

SpritesImagePreview::SpritesImagePreview(int spriteAmount, QByteArray header, QVector<QByteArray> contents, int headerValue, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpritesImagePreview)
{
    ui->setupUi(this);

    qint16 x = 0;
    qint16 y = 0;
    qint8 formatType = 0;

    if (headerValue == NSppData || headerValue == NSmpData)
    {
        x = byteArrayToShort(content.mid(1, 3));
        y = byteArrayToShort(content.mid(3, 5));

        content.remove(0, 13);
    }

    QImage image = createQImage(x, y, formatType, content);
    ui->spriteListWidget->addItem();
    ui->imgContent->setPixmap(QPixmap::fromImage(image));

}

SpritesImagePreview::~SpritesImagePreview()
{
    delete ui;
}

qint16 SpritesImagePreview::byteArrayToShort(QByteArray array)
{
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

QImage SpritesImagePreview::createQImage(int w, int h, qint8 formatType, QByteArray& content)
{
    return converter.getFromData(content, w, h, static_cast<ImageConverter::ImageTypes>(formatType));
}

void SpritesImagePreview::showWarningMessage()
{
    QMessageBox::warning(this, "Not supported", "This file cannot be opened as sprite list.");
}
