#include "NosTextOpener.h"

NosTextOpener::NosTextOpener() {
    datDecryptor = new NosTextDatFileDecryptor();
    lstDecryptor = new NosTextOthersFileDecryptor(&littleEndianConverter);
}

OnexTreeItem *NosTextOpener::decrypt(QFile &file) {
    file.seek(0);
    auto *item = new OnexTreeText(neatFileName(file.fileName()), this);
    int fileAmount = littleEndianConverter.readInt(file);
    for (int i = 0; i < fileAmount; ++i) {
        int fileNumber = littleEndianConverter.readInt(file);
        int stringNameSize = littleEndianConverter.readInt(file);
        QString stringName = file.read(stringNameSize);
        int isDat = littleEndianConverter.readInt(file);
        int fileSize = littleEndianConverter.readInt(file);
        QByteArray fileContent = file.read(fileSize);
        QByteArray decryptedArray;
        if (isDat || stringName.endsWith(".dat"))
            decryptedArray = datDecryptor->decrypt(fileContent);
        else    //.lst
            decryptedArray = lstDecryptor->decrypt(fileContent);
        item->addChild(new OnexTreeText(stringName, this, fileNumber, isDat, decryptedArray));
    }
    return item;
}

QByteArray NosTextOpener::encrypt(OnexTreeItem *item) {
    if (item->hasParent())
        return QByteArray();
    QByteArray result;
    result.push_back(littleEndianConverter.toInt(item->childCount()));
    for (int i = 0; i < item->childCount(); ++i) {
        auto *currentItem = dynamic_cast<OnexTreeText *>(item->child(i));
        result.push_back(littleEndianConverter.toInt(currentItem->getFileNumber()));
        result.push_back(littleEndianConverter.toInt(currentItem->getName().size()));
        result.push_back(currentItem->getName().toLocal8Bit());
        result.push_back(littleEndianConverter.toInt(currentItem->getIsDat()));
        QList<QByteArray> splited;
        QByteArray encrypted;
        if (currentItem->getIsDat() || currentItem->getName().endsWith(".dat")) {
            splited = currentItem->getContent().split(0xD);
        } else {
            splited = currentItem->getContent().split(0xA);
            encrypted.push_back(littleEndianConverter.toInt(splited.size() - 1));
        }
        for (int line = 0; line < splited.size() - 1; ++line) {
            if (currentItem->getIsDat() || currentItem->getName().endsWith(".dat"))
                encrypted.push_back(datDecryptor->encrypt(splited[line]));
            else
                encrypted.push_back(lstDecryptor->encrypt(splited[line]));
        }
        result.push_back(littleEndianConverter.toInt(encrypted.size()));
        result.push_back(encrypted);
    }
    return result;
}

OnexTreeItem *NosTextOpener::getEmptyItem(const QByteArray &header) {
    return new OnexTreeText("", this);
}
