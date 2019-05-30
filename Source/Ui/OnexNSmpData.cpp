#include "OnexNSmpData.h"
#include "OnexNSmpFrame.h"

OnexNSmpData::OnexNSmpData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) : OnexTreeZlibItem(name, content, opener, id, creationDate, compressed)
{
}

QByteArray OnexNSmpData::getContent()
{
    int amount = this->childCount();

    if ( amount <= 0 ) 
        return content;

    QByteArray offsetArray;

    offsetArray.push_back((uint8_t)amount);

    int sizeOfOffsetArray = 1 + amount * 12;

    QByteArray contentArray;

    for (int i = 0; i < amount; i++)
    {
        int currentFileOffset = sizeOfOffsetArray + contentArray.size();
        OnexNSmpFrame *currentItem = static_cast<OnexNSmpFrame *>(this->child(i));
        offsetArray.push_back(writeShort(currentItem->getWidth()));
        offsetArray.push_back(writeShort(currentItem->getHeight()));
        offsetArray.push_back(writeShort(currentItem->getXOrigin()));
        offsetArray.push_back(writeShort(currentItem->getYOrigin()));
        contentArray.push_back(currentItem->getContent());
        offsetArray.push_back(writeInt(currentFileOffset));
    }

    this->content = QByteArray();
    content.push_back(offsetArray);
    content.push_back(contentArray);

    return content;
}

QWidget *OnexNSmpData::onClicked()
{
    int amount = content.at(0);
    for (int i = 0; i < amount; i++)
    {
        int width = fromLittleEndianToShort((content.mid(1 + i * 12, 2).data()));
        int height = fromLittleEndianToShort((content.mid(3 + i * 12, 2).data()));
        int xOrigin = fromLittleEndianToShort((content.mid(5 + i * 12, 2).data()));
        int yOrigin = fromLittleEndianToShort((content.mid(7 + i * 12, 2).data()));
        int offset = qFromLittleEndian<int>(reinterpret_cast<const uchar *>((content.mid(9 + i * 12, 4).data())));

        QByteArray subContent = content.mid(offset, (height * 2 * width + width * 2 + 1));
        this->addChild(new OnexNSmpFrame(name + "_" + QString::number(i), subContent, width, height, xOrigin, yOrigin, opener, id, creationDate, compressed));
    }
    this->setExpanded(true);
    return nullptr;
}

QByteArray OnexNSmpData::writeShort(short number)
{
    QByteArray writeArray;
    writeArray.resize(2);
    qToLittleEndian<short>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray OnexNSmpData::writeInt(int number)
{
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<int>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

OnexNSmpData::~OnexNSmpData()
{
}
