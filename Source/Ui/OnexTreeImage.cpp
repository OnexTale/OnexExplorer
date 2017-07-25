#include "OnexTreeImage.h"

OnexTreeImage::OnexTreeImage(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeZlibItem(name, content, opener, id, creationDate, compressed)
{

}

bool OnexTreeImage::hasGoodResolution(int x, int y)
{
    ImageResolution currentResolution = this->getResolution();
    return (x == currentResolution.x*2 && y == currentResolution.y*2);
}

QImage OnexTreeImage::importQImageFromSelectedUserFile()
{
    QString fileName = getOpenDirectory("PNG Image (*.png)");
    if (fileName.isEmpty())
        return QImage();

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QImage image = QImage::fromData(file.readAll());
    file.close();

    return image;
}

QWidget *OnexTreeImage::onClicked()
{
    SingleImagePreview* imagePreview = new SingleImagePreview(this->getImage());
    imagePreview->setWindowTitle(this->getName());

    connect(this, SIGNAL(replaceSignal(QImage)), imagePreview, SLOT(onReplaced(QImage)));

    return imagePreview;
}

void OnexTreeImage::onExportAll()
{
    QString directory = this->getSelectedDirectory();
    if (directory.isEmpty())
        return;

    int count = 0;
    for (int i = 0; i != this->childCount(); ++i)
    {
        OnexTreeImage* item = static_cast<OnexTreeImage*>(this->child(i));
        if (item->getImage().save(directory + item->getName() + ".png", "PNG", 100))
            count++;
    }
    QString text = "Saved " + QString::number(count) + " of " + QString::number(this->childCount()) + " files.";
    QMessageBox msgBox(QMessageBox::Information, tr("End of operation"), text);
    msgBox.exec();
}

void OnexTreeImage::onExportSingle()
{
    QString fileName = getSaveDirectory("PNG Image (*.png)");

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".png"))
        fileName += ".png";

    if (this->getImage().save(fileName, "PNG", 100))
        QMessageBox::information(NULL, "Yeah", "Image exported");
    else
        QMessageBox::critical(NULL, "Woops", "Couldn't export that image");
}
