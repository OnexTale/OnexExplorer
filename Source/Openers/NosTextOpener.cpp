#include "NosTextOpener.h"

NosTextOpener::NosTextOpener()
{

}

OnexTreeItem *NosTextOpener::decrypt(QFile &file)
{
    file.seek(0);

    OnexTreeText *item = new OnexTreeText(neatFileName(file.fileName()), this);
    int fileAmount = readNextInt(file);

    for (int i = 0; i < fileAmount; ++i)
    {
        int fileNumber = readNextInt(file);
        int stringNameSize = readNextInt(file);
        QString stringName = file.read(stringNameSize);
        int isDat = readNextInt(file);
        int fileSize = readNextInt(file);
        QByteArray fileContent = file.read(fileSize);
        QByteArray decryptedArray;
        if (isDat)
            decryptedArray = datDecryptor.decrypt(fileContent);
        else    //.lst
            decryptedArray = lstDecryptor.decrypt(fileContent);

        item->addChild(new OnexTreeText(stringName, this, fileNumber, isDat, decryptedArray));
    }

    return item;
}

QByteArray NosTextOpener::encrypt(OnexTreeItem *item)
{
    if (item->hasParent())
        return QByteArray();

    QByteArray result;
    result.push_back(writeNextInt(item->childCount()));

    for (int i = 0; i < item->childCount(); ++i)
    {
        OnexTreeText* currentItem = static_cast<OnexTreeText*>(item->child(i));
        result.push_back(writeNextInt(currentItem->getFileNmber()));
        result.push_back(writeNextInt(currentItem->getName().size()));
        result.push_back(currentItem->getName().toLocal8Bit());
        result.push_back(writeNextInt(currentItem->getIsDat()));

        QByteArray encrypted;
        QList<QByteArray> splited = currentItem->getContent().split(0xD);
        for (int line = 0; line < splited.size(); ++line)
            encrypted.push_back(datDecryptor.encrypt(splited[line]));

        result.push_back(writeNextInt(encrypted.size()));
        result.push_back(encrypted);
    }

    return result;
}
