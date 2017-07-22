#include "NosTextOpener.h"

NosTextOpener::NosTextOpener()
{

}

OnexTreeItem *NosTextOpener::decrypt(QFile &file)
{
    file.seek(0);

    OnexTreeText *item = new OnexTreeText(neatFileName(file.fileName()));
    int fileAmount = readNextInt(file);

    for (int i = 0; i < fileAmount; ++i)
    {
        int fileNumber = readNextInt(file);
        int stringNameSize = readNextInt(file);
        QString stringName = file.read(stringNameSize);
        int isDat = readNextInt(file);
        int fileSize = readNextInt(file);
        QByteArray fileContent = file.read(fileSize);

        QByteArray decryptedArray = datDecryptor.decrypt(fileContent);

        item->addChild(new OnexTreeText(stringName, fileNumber, isDat, decryptedArray));
    }

    return item;
}

QByteArray NosTextOpener::encrypt(OnexTreeItem *item)
{
    return QByteArray();
}
