#include "OnexTreeImage.h"

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

QWidget *OnexTreeImage::getInfos() {
    if (!hasParent())
        return nullptr;
    QWidget *w = OnexTreeZlibItem::getInfos();
    QGridLayout *infoLayout = static_cast<QGridLayout *>(w->layout());

    ImageResolution ir = getResolution();

    QLabel *widthLabel = new QLabel("Width");
    infoLayout->addWidget(widthLabel, 5, 0);
    QLineEdit *widthIn = new QLineEdit(QString::number(ir.x));
    //    connect(nameIn, SIGNAL(textChanged(QString)), this, SLOT(test(QString)));
    infoLayout->addWidget(widthIn, 5, 1);

    QLabel *heightLabel = new QLabel("Height");
    infoLayout->addWidget(heightLabel, 6, 0);
    QLineEdit *heightIn = new QLineEdit(QString::number(ir.y));
    //    connect(nameIn, SIGNAL(textChanged(QString)), this, SLOT(test(QString)));
    infoLayout->addWidget(heightIn, 6, 1);

    return w;
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
        QString path = directory + this->getName() + ".png";
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