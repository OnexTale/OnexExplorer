#include "OnexTreeImage.h"

ImageConverter OnexTreeImage::imageConverter = ImageConverter();

OnexTreeImage::OnexTreeImage(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                             int creationDate, bool compressed)
    : OnexTreeZlibItem(header, name, content, opener, id, creationDate, compressed) {
}

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

QWidget *OnexTreeImage::getPreview() {
    if (!hasParent())
        return nullptr;
    SingleImagePreview *imagePreview = new SingleImagePreview(this->getImage());

    connect(this, SIGNAL(replaceSignal(QImage)), imagePreview, SLOT(onReplaced(QImage)));

    return imagePreview;
}

FileInfo *OnexTreeImage::getInfos() {
    if (!hasParent())
        return nullptr;
    FileInfo *infos = OnexTreeZlibItem::getInfos();

    ImageResolution ir = getResolution();

    connect(infos->addIntLineEdit("Width", ir.x), &QLineEdit::textChanged,
            [=](const QString &value) { setWidth(value.toInt()); });
    connect(infos->addIntLineEdit("Height", ir.y), &QLineEdit::textChanged,
            [=](const QString &value) { setHeight(value.toInt()); });

    return infos;
}

int OnexTreeImage::onExport(QString directory) {
    if (childCount() > 0) {
        int count = 0;
        for (int i = 0; i != this->childCount(); ++i) {
            OnexTreeImage *item = static_cast<OnexTreeImage *>(this->child(i));
            count += item->onExport(directory);
        }
        return count;
    } else {
        QString path = "";
        if (!directory.endsWith(".png"))
            path = directory + this->name + ".png";
        else
            path = directory;
         
        if (this->getImage().save(path, "PNG", 100))
            return 1;
        else if (this->getResolution().x == 0 || this->getResolution().y == 0) {
            QFile file(path);
            if (file.open(QIODevice::WriteOnly)) {
                file.close();
                return 1;
            }
        }
    }
    return 0;
}

QString OnexTreeImage::getExportExtension() {
    return ".png";
}