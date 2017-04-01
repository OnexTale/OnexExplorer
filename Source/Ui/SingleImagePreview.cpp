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

    if (headerValue == NSipData)
    {
        formatType = 0;
        x = byteArrayToShort(content.mid(1, 3));
        y = byteArrayToShort(content.mid(3, 5));

        content.remove(0, 13);
    }
    else if (headerValue == NStpData)
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
    return converter.getFromData(content, w, h, static_cast<ImageConverter::ImageTypes>(formatType));
}

void SingleImagePreview::showWarningMessage()
{
    QMessageBox::warning(this, "Not supported", "This file cannot be opened as image.");
}
