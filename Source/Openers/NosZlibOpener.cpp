#include "NosZlibOpener.h"

QByteArray NosZlibOpener::toBigEndian(qint32 value)
{
    QByteArray result;
    result.resize(4);

    qToBigEndian(value, reinterpret_cast<uchar*>(result.data()));

    return result;
}

NosZlibOpener::NosZlibOpener()
{

}

OnexTreeItem *NosZlibOpener::decrypt(QFile &file)
{
    file.seek(0);

    QByteArray header = file.read(NOS_HEADER_SIZE);

    int fileAmount = readNextInt(file);

    QStringList split = file.fileName().split("/");
    OnexTreeItem *item = new OnexTreeItem(split.at(split.size() - 1));

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
           qDebug() << QString::number(id) + ".RAW compressed offset: " + QString::number(offset);
           QByteArray bigEndian = toBigEndian(compressedDataSize);
           data.push_front(bigEndian);
           data = decryptor.decrypt(data);
        }
        else
        {
           qDebug() << QString::number(id) + ".RAW NOT COMPRESSED offset: " + QString::number(offset);
        }

        item->addChild(new OnexTreeItem(QString::number(id), data));
        file.seek(previousOffset);

    }

    return item;
}
