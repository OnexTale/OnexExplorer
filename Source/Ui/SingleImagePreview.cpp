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

void SingleImagePreview::onReplaced(QImage newImage)
{
    ui->imgContent->setPixmap(QPixmap::fromImage(newImage));
}

