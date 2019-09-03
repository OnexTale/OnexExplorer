#include <QtCore/QDate>
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

    if (file.bytesAvailable() == 12) {
        file.seek(file.pos() + 8);
        if (file.read(4) == QByteArrayLiteral("\xEE\x3E\x32\x01")) {
            file.seek(file.pos() - 12);
            QString time = readOLETIME(file.read(8));
            item->setTime(time, true);
        }
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
        result.push_back(littleEndianConverter.toInt(currentItem->getIsCompressed()));
        QList<QByteArray> splited;
        QByteArray encrypted;
        if (currentItem->getIsCompressed() || currentItem->getName().endsWith(".dat")) {
            splited = currentItem->getContent().split(0xD);
        } else {
            splited = currentItem->getContent().split(0xA);
            encrypted.push_back(littleEndianConverter.toInt(splited.size() - 1));
        }
        for (auto &line : splited) {
            if (line.isEmpty())
                continue;
            if (currentItem->getIsCompressed() || currentItem->getName().endsWith(".dat"))
                encrypted.push_back(datDecryptor->encrypt(line));
            else
                encrypted.push_back(lstDecryptor->encrypt(line));
        }
        result.push_back(littleEndianConverter.toInt(encrypted.size()));
        result.push_back(encrypted);
    }

    QByteArray time = generateOLETIME();
    result.push_back(time);
    result.push_back(QByteArrayLiteral("\xEE\x3E\x32\x01"));

    auto *text = static_cast<OnexTreeText *>(item);
    text->setTime(readOLETIME(time), true);

    return result;
}

OnexTreeItem *NosTextOpener::getEmptyItem(const QByteArray &header) {
    return new OnexTreeText("", this);
}

QByteArray NosTextOpener::generateOLETIME() {
    QDateTime dt = QDateTime::currentDateTime();
    dt.setTimeSpec(Qt::TimeSpec::UTC);
    double variant = ((dt.toSecsSinceEpoch() + 2208988800) / 86400.0) + 2.00001;
    QByteArray writeArray;
    writeArray.resize(8);
    qToLittleEndian<double>(variant, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QString NosTextOpener::readOLETIME(QByteArray array) {
    double variant = qFromLittleEndian<double>(reinterpret_cast<const uchar *>(array.data()));
    unsigned long unixTime = -2208988800 + ((variant - 2.00001) * 86400);
    QDateTime dt;
    dt.setTimeSpec(Qt::TimeSpec::UTC);
    dt.setSecsSinceEpoch(unixTime);
    return dt.toString("dd/MM/yyyy hh:mm:ss");
}