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
