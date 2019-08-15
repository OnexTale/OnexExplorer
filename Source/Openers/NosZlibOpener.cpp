#include "NosZlibOpener.h"
#include "../Ui/TreeItems/OnexNS4BbData.h"
#include "../Ui/TreeItems/OnexNSipData.h"
#include "../Ui/TreeItems/OnexNSmpData.h"
#include "../Ui/TreeItems/OnexNStcData.h"
#include "../Ui/TreeItems/OnexNStpData.h"
#include "../Ui/TreeItems/OnexNStgData.h"
#include "../Ui/TreeItems/OnexTreeZlibItem.h"

NosZlibOpener::NosZlibOpener() = default;

OnexTreeItem *NosZlibOpener::decrypt(QFile &file) {
    file.seek(0);
    QByteArray header = file.read(NOS_HEADER_SIZE);
    int fileAmount = littleEndianConverter.readInt(file);
    OnexTreeItem *item = createItemFromHeader(header, neatFileName(file.fileName()), header);
    QByteArray separatorByte = file.read(1);
    for (int i = 0; i != fileAmount; ++i) {
        int id = littleEndianConverter.readInt(file);
        int offset = littleEndianConverter.readInt(file);
        int previousOffset = file.pos();
        file.seek(offset);
        int creationDate = littleEndianConverter.readInt(file);
        int dataSize = littleEndianConverter.readInt(file);
        int compressedDataSize = littleEndianConverter.readInt(file);
        bool isCompressed = file.read(1).at(0);
        QByteArray data = file.read(compressedDataSize);
        if (isCompressed) {
            QByteArray bigEndian = toBigEndian(dataSize);
            data.push_front(bigEndian);
            data = decryptor.decrypt(data);
        }
        item->addChild(createItemFromHeader(header, QString::number(id), data, id, creationDate, isCompressed));
        file.seek(previousOffset);
    }
    return item;
}

QByteArray NosZlibOpener::encrypt(OnexTreeItem *item) {
    if (item->hasParent())
        return QByteArray();
    QByteArray fileHeader = item->getContent();
    fileHeader.push_back(littleEndianConverter.toInt(item->childCount()));
    fileHeader.push_back((char) 0x0); // separator byte

    QByteArray offsetArray;
    int sizeOfOffsetArray = item->childCount() * 8;
    QByteArray contentArray;
    int firstFileOffset = fileHeader.size() + sizeOfOffsetArray;
    for (int i = 0; i != item->childCount(); ++i) {
        int currentFileOffset = firstFileOffset + contentArray.size();
        auto *currentItem = dynamic_cast<OnexTreeZlibItem *>(item->child(i));
        contentArray.push_back(littleEndianConverter.toInt(currentItem->getCreationDate()));
        QByteArray content = currentItem->getContent();
        contentArray.push_back(littleEndianConverter.toInt(content.size()));
        if (currentItem->isCompressed()) {
            int headerNumber = getNTHeaderNumber(fileHeader);
            if (headerNumber == NS4BbData || headerNumber == NStcData || headerNumber == NStuData)
                content = decryptor.encrypt(content, 9);
            else
                content = decryptor.encrypt(content);
        }
        int compressedContentSize = content.size();
        if (currentItem->isCompressed())
            compressedContentSize -= 4; // qCompress add the size at the front of array

        contentArray.push_back(littleEndianConverter.toInt(compressedContentSize));
        contentArray.push_back(currentItem->isCompressed());
        if (currentItem->isCompressed())
            contentArray.push_back(content.mid(4));
        else
            contentArray.push_back(content);
        offsetArray.push_back(littleEndianConverter.toInt(currentItem->getId()));
        offsetArray.push_back(littleEndianConverter.toInt(currentFileOffset));
    }
    QByteArray result;
    result.push_back(fileHeader);
    result.push_back(offsetArray);
    result.push_back(contentArray);
    return result;
}

OnexTreeItem *NosZlibOpener::createItemFromHeader(QByteArray header, const QString &name, QByteArray &array, int fileId,
                                                  int creationDate, bool compressed) {
    int headerNumber = getNTHeaderNumber(header);
    switch (headerNumber) {
        case NSipData:
            return new OnexNSipData(header, name, array, this, fileId, creationDate, compressed);
        case NS4BbData:
            return new OnexNS4BbData(header, name, array, this, fileId, creationDate, compressed);
        case NStcData:
            return new OnexNStcData(header, name, array, this, fileId, creationDate, compressed);
        case NStpData:
        case NStpeData:
        case NStpuData:
            return new OnexNStpData(header, name, array, this, fileId, creationDate, compressed);
        case NSmpData:
        case NSppData:
            return new OnexNSmpData(header, name, array, this, fileId, creationDate, compressed);
        case NStgData:
        case NStgeData:
            return new OnexNStgData(header, name, array, this, fileId, creationDate, compressed);
        default:
            return new OnexTreeZlibItem(header, name, array, this, fileId, creationDate, compressed);
    }
}

int NosZlibOpener::getNTHeaderNumber(QByteArray &array) {
    if (array.mid(0, 7) == "NT Data")
        return array.mid(8, 2).toInt();
    else if (array.mid(0, 10) == "32GBS V1.0")
        return 101;
    else if (array.mid(0, 10) == "ITEMS V1.0")
        return 103;
    else
        return 199;
}

QByteArray NosZlibOpener::toBigEndian(qint32 value) {
    QByteArray result;
    result.resize(4);
    qToBigEndian(value, reinterpret_cast<uchar *>(result.data()));
    return result;
}
