#include "OnexTreeImage.h"

ImageConverter *OnexTreeImage::imageConverter;

OnexTreeImage::OnexTreeImage(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                             int creationDate, bool compressed)
        : OnexTreeZlibItem(header, name, content, opener, id, creationDate, compressed) {
    if (imageConverter == nullptr)
        imageConverter = new ImageConverter(opener->getLittleEndianConverter());
}

QWidget *OnexTreeImage::getPreview() {
    if (!hasParent())
        return nullptr;
    auto *imagePreview = new SingleImagePreview(this->getImage());
    connect(this, SIGNAL(replaceSignal(QImage)), imagePreview, SLOT(onReplaced(QImage)));
    return imagePreview;
}

FileInfo *OnexTreeImage::getInfos() {
    if (!hasParent())
        return nullptr;
    FileInfo *infos = generateInfos();
    connect(this, SIGNAL(replaceInfo(FileInfo * )), infos, SLOT(replace(FileInfo * )));
    return infos;
}

QString OnexTreeImage::getExportExtension() {
    return ".png";
}

int OnexTreeImage::onReplaceRaw(QString directory) {
    int ret = OnexTreeItem::onReplaceRaw(directory);
    emit replaceSignal(getImage());
    emit replaceInfo(generateInfos());
    return ret;
}

int OnexTreeImage::afterReplace(QByteArray content) {
    if (content.isEmpty() && childCount() > 0)
        return afterReplace(QImage());
    QImage image = QImage::fromData(content);
    if (image.isNull() && this->getResolution().x != 0 && this->getResolution().y != 0) {
        QMessageBox::critical(nullptr, "Woops", "Couldn't read image " + name);
        return 0;
    }
    if (!hasGoodResolution(image.width(), image.height())) {
        QMessageBox::StandardButton reply = QMessageBox::question(
                nullptr, "Resolution changed",
                "The resolution of the image " + name + " doesn't match!\nDo you want to replace it anyway?");
        if (reply == QMessageBox::No)
            return 0;
    }
    return afterReplace(image);
}

FileInfo *OnexTreeImage::generateInfos() {
    FileInfo *infos = OnexTreeZlibItem::generateInfos();
    ImageResolution ir = getResolution();
    connect(infos->addIntLineEdit("Width", ir.x), &QLineEdit::textChanged,
            [=](const QString &value) { setWidth(value.toInt()); });
    connect(infos->addIntLineEdit("Height", ir.y), &QLineEdit::textChanged,
            [=](const QString &value) { setHeight(value.toInt()); });
    return infos;
}

int OnexTreeImage::saveAsFile(const QString &path, QByteArray content) {
    if (this->getImage().save(path, "PNG", 100))
        return 1;
    else if (this->getResolution().x == 0 || this->getResolution().y == 0) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly)) {
            file.close();
            return 1;
        }
    }
    return 0;
}

bool OnexTreeImage::hasGoodResolution(int x, int y) {
    ImageResolution currentResolution = this->getResolution();
    return (x == currentResolution.x && y == currentResolution.y);
}
