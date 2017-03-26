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
        int offest = readNextInt(file);

        item->addChild(new OnexTreeItem(QString::number(id) + ".RAW"));

    }

    return item;
}
