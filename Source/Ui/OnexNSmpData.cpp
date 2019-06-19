#include "OnexNSmpData.h"
#include "OnexNSmpFrame.h"

OnexNSmpData::OnexNSmpData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeZlibItem(name, content, opener, id, creationDate, compressed) {
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
        this->addChild(new OnexNSmpFrame(name + "_" + QString::number(i), subContent, width, height, xOrigin, yOrigin,
                                         opener, id, creationDate, compressed));
    }
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

int OnexNSmpData::onExportAll(QString directory) {
    int count = 0;
    if (!hasParent()) {
        for (int i = 0; i != this->childCount(); ++i) {
            OnexNSmpData *item = static_cast<OnexNSmpData *>(this->child(i));
            count += exportFrames(item, directory);
        }
    } else
        count = exportFrames(this, directory);

    return count;
}

int OnexNSmpData::onExportSingle(QString directory) { return onExportAll(directory); }

int OnexNSmpData::onReplace(QString directory) {
    int count = 0;
    if (!hasParent()) {
        for (int i = 0; i != this->childCount(); ++i) {
            OnexNSmpData *item = static_cast<OnexNSmpData *>(this->child(i));
            count += replaceFrames(item, directory);
        }
    } else
        count = replaceFrames(this, directory);

    return count;
}

OnexNSmpData::~OnexNSmpData() {}

int OnexNSmpData::exportFrames(OnexNSmpData *src, QString directory) {
    int count = 0;
    for (int i = 0; i != src->childCount(); ++i) {
        OnexNSmpFrame *item = static_cast<OnexNSmpFrame *>(src->child(i));
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

int OnexNSmpData::replaceFrames(OnexNSmpData *src, QString directory) {
    int count = 0;
    for (int i = 0; i != src->childCount(); ++i) {
        OnexNSmpFrame *item = static_cast<OnexNSmpFrame *>(src->child(i));
        count += item->onReplace(directory);
    }
    return count;
}