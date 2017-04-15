#include "SingleImagePreview.h"
#include "ui_SingleImagePreview.h"

SingleImagePreview::SingleImagePreview(QByteArray content, int headerValue, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleImagePreview)
{
    ui->setupUi(this);

    qint16 w = 0;
    qint16 h = 0;
    qint8 formatType = 0;

    if (headerValue == NSipData)
    {
        formatType = 0;
        w = byteArrayToShort(content.mid(1, 3));
        h = byteArrayToShort(content.mid(3, 5));

        content.remove(0, 13);
    }
    else if (headerValue == NStpData || headerValue == NStpeData || headerValue == NStpuData)
    {
        w = byteArrayToShort(content.mid(0, 2));
        h = byteArrayToShort(content.mid(2, 2));

        formatType = content.at(4);
        content.remove(0, 8);
    }
    else if (headerValue == NStcData)
    {
        formatType = 3;
        w = byteArrayToShort(content.mid(0, 2));
        h = byteArrayToShort(content.mid(2, 2));

        content.remove(0, 4);
    }
    else
    {
        showWarningMessage();
        //need to stop displaying empty image form!
        return;
    }


    QImage image = createQImage(w, h, formatType, content);
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
