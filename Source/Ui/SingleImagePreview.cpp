#include "SingleImagePreview.h"
#include "ui_SingleImagePreview.h"

SingleImagePreview::SingleImagePreview(QByteArray content, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleImagePreview)
{
    ui->setupUi(this);

    qint16 x = byteArrayToShort(content.mid(0, 2));
    qint16 y = byteArrayToShort(content.mid(2, 2));;

    qint8 formatType = content.at(4);
    content.remove(0, 8);

    QImage image = createQImage(x, y, formatType, content);
    ui->imgContent->setPixmap(QPixmap::fromImage(image));

}

SingleImagePreview::~SingleImagePreview()
{
    delete ui;
}

qint16 SingleImagePreview::byteArrayToShort(QByteArray array)
{
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

QImage SingleImagePreview::createQImage(int w, int h, qint8 formatType, QByteArray& content)
{
    QImage::Format format = QImage::Format_Invalid;
    bool invert = false;
    switch (formatType) {
    case 0:
        format = QImage::Format_RGB444;
        break;
    case 1:
        format = QImage::Format_RGB555;
        break;
    case 2:
        format = QImage::Format_RGBA8888;
        invert = true;
        break;

    default:
        break;
    }

   QImage image((uchar*)content.data(), w, h, format);
   if (invert)
       return image.rgbSwapped();

   return image;
}
