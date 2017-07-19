#include "OnexTreeImage.h"

OnexTreeImage::OnexTreeImage(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeZlibItem(name, content, opener, id, creationDate, compressed)
{

}

QWidget *OnexTreeImage::onClicked()
{
    SingleImagePreview* imagePreview = new SingleImagePreview(this->getImage());
    imagePreview->setWindowTitle(this->getName());

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
