#include "OnexNSmpData.h"
#include "../Previews/MultiImagePreview.h"
#include "OnexNSmpFrame.h"

OnexNSmpData::OnexNSmpData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeZlibItem(header, name, content, opener, id, creationDate, compressed) {
    if (id == -1)
        return;
    int amount = content.at(0);
    for (int i = 0; i < amount; i++) {
        int width = fromLittleEndianToShort(content.mid(1 + i * 12, 2));
        int height = fromLittleEndianToShort(content.mid(3 + i * 12, 2));
        int xOrigin = fromLittleEndianToShort(content.mid(5 + i * 12, 2));
        int yOrigin = fromLittleEndianToShort(content.mid(7 + i * 12, 2));
        int offset = fromLittleEndianToInt(content.mid(9 + i * 12, 4));

        QByteArray subContent = content.mid(offset, (width * 2 * height));
        this->addChild(new OnexNSmpFrame(header, name + "_" + QString::number(i), subContent, width, height, xOrigin, yOrigin,
                                         opener, id, creationDate, compressed));
    }
}

QWidget *OnexNSmpData::getPreview() {
    if (!hasParent())
        return nullptr;

    QList<QImage> *images = new QList<QImage>();
    for (int i = 0; i != this->childCount(); ++i) {
        OnexNSmpFrame *item = static_cast<OnexNSmpFrame *>(this->child(i));
        images->append(item->getImage());
    }

    MultiImagePreview *imagePreview = new MultiImagePreview(images);

    return imagePreview;
}

QByteArray OnexNSmpData::getContent() {
    int amount = childCount();
    if (!hasParent() || amount <= 0)
        return content;

    QByteArray offsetArray;
    offsetArray.push_back((uint8_t)amount);
    int sizeOfOffsetArray = 1 + amount * 12;

    QByteArray contentArray;

    for (int i = 0; i < amount; i++) {
        int currentFileOffset = sizeOfOffsetArray + contentArray.size();
        OnexNSmpFrame *currentItem = static_cast<OnexNSmpFrame *>(this->child(i));
        offsetArray.push_back(fromShortToLittleEndian(currentItem->getWidth()));
        offsetArray.push_back(fromShortToLittleEndian(currentItem->getHeight()));
        offsetArray.push_back(fromShortToLittleEndian(currentItem->getXOrigin()));
        offsetArray.push_back(fromShortToLittleEndian(currentItem->getYOrigin()));
        offsetArray.push_back(fromIntToLittleEndian(currentFileOffset));
        contentArray.push_back(currentItem->getContent());
    }

    this->content = QByteArray();
    content.push_back(offsetArray);
    content.push_back(contentArray);

    return content;
}

int OnexNSmpData::onExport(QString directory) {
    int count = 0;
    for (int i = 0; i != this->childCount(); ++i) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(this->child(i));
        count += item->onExport(directory);
    }
    return count;
}

int OnexNSmpData::onReplace(QString directory) {
    int count = 0;
    for (int i = 0; i != this->childCount(); ++i) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(this->child(i));
        count += item->onReplace(directory);
    }
    return count;
}

OnexNSmpData::~OnexNSmpData() {
}