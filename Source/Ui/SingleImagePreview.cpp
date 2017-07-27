#include "SingleImagePreview.h"
#include "ui_SingleImagePreview.h"

SingleImagePreview::SingleImagePreview(QImage image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleImagePreview)
{
    ui->setupUi(this);
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
    Q_UNUSED(pos);

    QMenu contextMenu(this);

    QAction exportAction("Export", this);
    connect(&exportAction, SIGNAL(triggered()), this, SLOT(exportImage()));
    contextMenu.addAction(&exportAction);

    contextMenu.exec(QCursor::pos());
}

void SingleImagePreview::exportImage()
{
    QString fileName = QFileDialog::getSaveFileName(0, tr("Save as..."), "", tr("PNG Image (*.png)"));

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".png"))
        fileName += ".png";

    if (ui->imgContent->pixmap()->toImage().save(fileName, "PNG", 100))
        QMessageBox::information(NULL, "Yeah", "Image exported");
    else
        QMessageBox::critical(NULL, "Woops", "Couldn't export that image");

}

void SingleImagePreview::onReplaced(QImage newImage)
{
    ui->imgContent->setPixmap(QPixmap::fromImage(newImage));
}

