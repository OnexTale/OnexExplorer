#include "OnexTreeImage.h"

OnexTreeImage::OnexTreeImage(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                             bool compressed)
    : OnexTreeZlibItem(name, content, opener, id, creationDate, compressed) {}

bool OnexTreeImage::hasGoodResolution(int x, int y) {
    ImageResolution currentResolution = this->getResolution();
    return (x == currentResolution.x && y == currentResolution.y);
}

QImage OnexTreeImage::importQImageFromSelectedUserFile(QString filepath) {
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QImage image = QImage::fromData(file.readAll());
    file.close();

    return image;
}

QWidget *OnexTreeImage::onClicked() {
    if (childCount() != 0)
        return nullptr;
    SingleImagePreview *imagePreview = new SingleImagePreview(this->getImage());
    imagePreview->setWindowTitle(this->getName());

    connect(this, SIGNAL(replaceSignal(QImage)), imagePreview, SLOT(onReplaced(QImage)));

    return imagePreview;
}

int OnexTreeImage::onExportAll(QString directory) {
    int count = 0;
    for (int i = 0; i != this->childCount(); ++i) {
        OnexTreeImage *item = static_cast<OnexTreeImage *>(this->child(i));
        if (item->getImage().save(directory + item->getName() + ".png", "PNG", 100))
            count++;
        else if (item->getResolution().x == 0 || item->getResolution().y == 0) {
            QFile file(directory + item->getName() + ".png");
            if (file.open(QIODevice::WriteOnly)) {
                count++;
                file.close();
            }
        }
    }
    return count;
}

int OnexTreeImage::onExportSingle(QString directory) {
    QString path = directory + this->getName() + ".png";
    if (this->getImage().save(path, "PNG", 100))
        return 1;
    else if (this->getResolution().x == 0 || this->getResolution().y == 0) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly)) {
            file.close();
            return 1;
        } else {
            QMessageBox::critical(NULL, "Woops", "Couldn't export that image");
            return 0;
        }
    } else {
        QMessageBox::critical(NULL, "Woops", "Couldn't export that image");
        return 1;
    }
}
