#include "OnexNSmpData.h"
#include "../Previews/MultiImagePreview.h"
#include "OnexNSmpFrame.h"

OnexNSmpData::OnexNSmpData(const QByteArray &header, const QString &name, const QByteArray &content, NosZlibOpener *opener,

                           int id, int creationDate, bool compressed)
        : OnexTreeZlibItem(header, name, content, opener, id, creationDate, compressed) {
    if (id == -1)
        return;
    int amount = content.at(0);
    for (int i = 0; i < amount; i++) {
        int width = opener->getLittleEndianConverter()->fromShort(content.mid(1 + i * 12, 2));
        int height = opener->getLittleEndianConverter()->fromShort(content.mid(3 + i * 12, 2));
        int xOrigin = opener->getLittleEndianConverter()->fromShort(content.mid(5 + i * 12, 2));
        int yOrigin = opener->getLittleEndianConverter()->fromShort(content.mid(7 + i * 12, 2));
        int offset = opener->getLittleEndianConverter()->fromInt(content.mid(9 + i * 12, 4));
        QByteArray subContent = content.mid(offset, (width * 2 * height));
        this->addChild(new OnexNSmpFrame(header, name + "_" + QString::number(i), subContent, width, height, xOrigin,
                                         yOrigin, opener, id, creationDate, compressed));
    }
}

OnexNSmpData::~OnexNSmpData() = default;

QWidget *OnexNSmpData::getPreview() {
    if (!hasParent())
        return nullptr;
    auto *images = new QList<QImage>();
    for (int i = 0; i != this->childCount(); ++i) {
        auto *item = dynamic_cast<OnexNSmpFrame *>(this->child(i));
        images->append(item->getImage());
    }
    auto *imagePreview = new MultiImagePreview(images);
    connect(this, SIGNAL(replaceSignal(QList<QImage> * )), imagePreview, SLOT(onReplaced(QList<QImage> * )));
    return imagePreview;
}

QByteArray OnexNSmpData::getContent() {
    int amount = childCount();
    if (!hasParent() || amount <= 0)
        return content;
    QByteArray offsetArray;
    offsetArray.push_back((uint8_t) amount);
    int sizeOfOffsetArray = 1 + amount * 12;
    QByteArray contentArray;
    for (int i = 0; i < amount; i++) {
        int currentFileOffset = sizeOfOffsetArray + contentArray.size();
        auto *currentItem = dynamic_cast<OnexNSmpFrame *>(this->child(i));
        offsetArray.push_back(opener->getLittleEndianConverter()->toShort(currentItem->getWidth()));
        offsetArray.push_back(opener->getLittleEndianConverter()->toShort(currentItem->getHeight()));
        offsetArray.push_back(opener->getLittleEndianConverter()->toShort(currentItem->getXOrigin()));
        offsetArray.push_back(opener->getLittleEndianConverter()->toShort(currentItem->getYOrigin()));
        offsetArray.push_back(opener->getLittleEndianConverter()->toInt(currentFileOffset));
        contentArray.push_back(currentItem->getContent());
    }
    this->content = QByteArray();
    content.push_back(offsetArray);
    content.push_back(contentArray);
    return content;
}

int OnexNSmpData::afterReplace(QByteArray content) {
    auto *images = new QList<QImage>();
    for (int i = 0; i != this->childCount(); ++i) {
        auto *item = dynamic_cast<OnexNSmpFrame *>(this->child(i));
        images->append(item->getImage());
    }
    emit replaceSignal(images);
    return 1;
}
