#include "NosZlibOpener.h"

QByteArray NosZlibOpener::toBigEndian(qint32 value)
{
    QByteArray result;
    result.resize(4);

    qToBigEndian(value, reinterpret_cast<uchar*>(result.data()));

    return result;
}

int NosZlibOpener::getNTHeaderNumber(QByteArray &array)
{
    if (array.mid(0, 2) == "NT")
        return array.mid(8, 2).toInt();
    else if (array.mid(0, 2) == "32")
        return 101;
    else //if (array.mid(0, 2) == "CC")
        return 102;
}

NosZlibOpener::NosZlibOpener()
{

}

OnexTreeItem *NosZlibOpener::decrypt(QFile &file)
{
    file.seek(0);

    QByteArray header = file.read(NOS_HEADER_SIZE);

    int ntHeaderNumber = getNTHeaderNumber(header);
    int fileAmount = readNextInt(file);

    OnexTreeItem *item = new OnexTreeItem(neatFileName(file.fileName()), NosEnumTypes::NOS_ARCHIVE);

    QByteArray separatorByte = file.read(1);

    for (int i = 0; i != fileAmount; ++i)
    {
        int id = readNextInt(file);
        int offset = readNextInt(file);

        int previousOffset = file.pos();

        file.seek(offset);

        int creationDate = readNextInt(file);
        int dataSize = readNextInt(file);
        int compressedDataSize = readNextInt(file);
        bool isCompressed = file.read(1).at(0);
        QByteArray data = file.read(dataSize);
        if (isCompressed)
        {
           QByteArray bigEndian = toBigEndian(compressedDataSize);
           data.push_front(bigEndian);
           data = decryptor.decrypt(data);
        }

        item->addChild(new OnexTreeItem(QString::number(id), NosEnumTypes::NOS_ARCHIVE, data, ntHeaderNumber, creationDate));
        file.seek(previousOffset);

    }

    return item;
}
