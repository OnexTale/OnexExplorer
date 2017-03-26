#include "NosZlibOpener.h"

NosZlibOpener::NosZlibOpener()
{

}

OnexTreeItem *NosZlibOpener::decrypt(QFile &file)
{
    file.seek(0);

    QByteArray header = file.read(NOS_HEADER_SIZE);

    int fileAmount = readNextInt(file);

    OnexTreeItem *item = new OnexTreeItem(file.fileName());

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
           qDebug() << QString::number(id) + ".RAW compressed";
        else
        {
           qDebug() << QString::number(id) + ".RAW NOT COMPRESSED";
        }

        item->addChild(new OnexTreeItem(QString::number(id) + ".RAW", data));
        file.seek(previousOffset);

    }

    return item;
}
