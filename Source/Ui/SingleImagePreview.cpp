#include "SingleImagePreview.h"
#include "ui_SingleImagePreview.h"

SingleImagePreview::SingleImagePreview(QByteArray content, int headerValue, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleImagePreview)
{
    ui->setupUi(this);

    qint16 x = 0;
    qint16 y = 0;
    qint8 formatType = 0;

    if (headerValue == 24)
    {
        formatType = 0;
        x = byteArrayToShort(content.mid(1, 3));
        y = byteArrayToShort(content.mid(3, 5));

        content.remove(0, 13);
    }
    else if (headerValue == 7)
    {
        x = byteArrayToShort(content.mid(0, 2));
        y = byteArrayToShort(content.mid(2, 2));;

        formatType = content.at(4);
        content.remove(0, 8);
    }
    else
    {
        showWarningMessage();
        return;
    }


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
        format = QImage::Format_ARGB4444_Premultiplied;
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

void SingleImagePreview::showWarningMessage()
{
    QMessageBox::warning(this, "Not supported", "This file cannot be opened as image.");
}
