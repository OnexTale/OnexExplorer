#include "SingleImagePreview.h"
#include "ui_SingleImagePreview.h"

SingleImagePreview::SingleImagePreview(QImage image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleImagePreview)
{
    ui->setupUi(this);

    /*
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
    else if (headerValue == NSipData2006)
    {
        formatType = 5;
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
    else if (headerValue == NS4BbData)
    {
        formatType = 4;
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

    QStringList fNames = { "GBAR4444", "ARGB555", "BGRA8888", "NSTC", "BGRA8888_INTERLACED", "BARG4444" };
    QString fName = "?";
    if (fNames.count() > formatType)
        fName = fNames.at(formatType);

    QImage image = createQImage(w, h, formatType, content);
    ui->imgContent->setPixmap(QPixmap::fromImage(image));
    this->setWindowTitle(QString::number(w) + "x" + QString::number(h) + ", " + fName);
    */

    ui->imgContent->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->imgContent, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCustomMenu(QPoint)));

    ui->imgContent->setPixmap(QPixmap::fromImage(image));

}

SingleImagePreview::~SingleImagePreview()
{
    delete ui;
}

void SingleImagePreview::showCustomMenu(const QPoint &pos)
{
    QMenu contextMenu(this);

    QAction exportAction("Export", this);
    connect(&exportAction, SIGNAL(triggered()), this, SLOT(exportImage()));
    contextMenu.addAction(&exportAction);

    contextMenu.exec(mapToGlobal(pos));
}

void SingleImagePreview::exportImage()
{
    QFileDialog dialog(this, tr("Save image as..."));
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save image as..."), QString(), tr("PNG image (.*png"));

    ui->imgContent->pixmap()->toImage().save(fileName, "PNG", 100);
}

