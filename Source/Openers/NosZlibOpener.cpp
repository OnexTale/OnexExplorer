#include "NosZlibOpener.h"
#include "../Ui/OnexNS4BbData.h"
#include "../Ui/OnexNSipData.h"
#include "../Ui/OnexNSmpData.h"
#include "../Ui/OnexNStcData.h"
#include "../Ui/OnexNStpData.h"
#include "../Ui/OnexTreeZlibItem.h"

QByteArray NosZlibOpener::toBigEndian(qint32 value) {
    QByteArray result;
    result.resize(4);

    qToBigEndian(value, reinterpret_cast<uchar *>(result.data()));

    return result;
}

int NosZlibOpener::getNTHeaderNumber(QByteArray &array) {
    if (array.mid(0, 7) == "NT Data")
        return array.mid(8, 2).toInt();
    else if (array.mid(0, 10) == "32GBS V1.0")
        return 101;
    // else if (array.mid(0, 11) == "CCINF V1.20")
    //    return 102;
    else if (array.mid(0, 10) == "ITEMS V1.0")
        return 103;
    else
        return 199;
}

OnexTreeItem *NosZlibOpener::createItemFromHeader(int header, QString name, QByteArray &array, int fileId,
                                                  int creationDate, bool compressed) {
    switch (header) {
    case NSipData:
        return new OnexNSipData(name, array, this, fileId, creationDate, compressed);
        break;

    case NS4BbData:
        return new OnexNS4BbData(name, array, this, fileId, creationDate, compressed);
        break;

    case NStcData:
        return new OnexNStcData(name, array, this, fileId, creationDate, compressed);
        break;

    case NStpData:
    case NStpeData:
    case NStpuData:
        return new OnexNStpData(name, array, this, fileId, creationDate, compressed);
        break;

    case NSmpData:
    case NSppData:
        return new OnexNSmpData(name, array, this, fileId, creationDate, compressed);
        break;

    default:
        return new OnexTreeZlibItem(name, array, this, fileId, creationDate, compressed);
        break;
    }
}

NosZlibOpener::NosZlibOpener() {}

ImageConverter &NosZlibOpener::getImageConverter() { return imageConverter; }

OnexTreeItem *NosZlibOpener::decrypt(QFile &file) {
    file.seek(0);

    QByteArray header = file.read(NOS_HEADER_SIZE);

    int ntHeaderNumber = getNTHeaderNumber(header);
    int fileAmount = readNextInt(file);

    OnexTreeItem *item = createItemFromHeader(ntHeaderNumber, neatFileName(file.fileName()), header);

    QByteArray separatorByte = file.read(1);

    for (int i = 0; i != fileAmount; ++i) {
        int id = readNextInt(file);
        int offset = readNextInt(file);

        int previousOffset = file.pos();

        file.seek(offset);

        int creationDate = readNextInt(file);
        int dataSize = readNextInt(file);
        int compressedDataSize = readNextInt(file);
        bool isCompressed = file.read(1).at(0);
        QByteArray data = file.read(compressedDataSize);
        if (isCompressed) {
            QByteArray bigEndian = toBigEndian(dataSize);
            data.push_front(bigEndian);
            data = decryptor.decrypt(data);
        }

        item->addChild(createItemFromHeader(ntHeaderNumber, QString::number(id), data, id, creationDate, isCompressed));
        file.seek(previousOffset);
    }

    // encrypt(item);

    return item;
}

QByteArray NosZlibOpener::encrypt(OnexTreeItem *item) {
    if (item->hasParent())
        return QByteArray();

    QByteArray fileHeader = item->getContent();
    fileHeader.push_back(writeNextInt(item->childCount()));
    fileHeader.push_back((char)0x0); // separator byte

    QByteArray offsetArray;
    int sizeOfOffsetArray = item->childCount() * 8;

    QByteArray contentArray;

    int firstFileOffset = fileHeader.size() + sizeOfOffsetArray;

    for (int i = 0; i != item->childCount(); ++i) {
        int currentFileOffset = firstFileOffset + contentArray.size();
        OnexTreeZlibItem *currentItem = static_cast<OnexTreeZlibItem *>(item->child(i));
        contentArray.push_back(writeNextInt(currentItem->getCreationDate()));
        QByteArray content = currentItem->getContent();
        contentArray.push_back(writeNextInt(content.size()));

        if (currentItem->isCompressed())
            content = decryptor.encrypt(content);

        int compressedContentSize = content.size();
        if (currentItem->isCompressed())
            compressedContentSize -= 4; // qCompress add the size at the front of array

        contentArray.push_back(writeNextInt(compressedContentSize));
        contentArray.push_back(currentItem->isCompressed());

        if (currentItem->isCompressed())
            contentArray.push_back(content.mid(4));
        else
            contentArray.push_back(content);

        offsetArray.push_back(writeNextInt(currentItem->getId()));
        offsetArray.push_back(writeNextInt(currentFileOffset));
    }

    QByteArray result;
    result.push_back(fileHeader);
    result.push_back(offsetArray);
    result.push_back(contentArray);

    return result;
}
