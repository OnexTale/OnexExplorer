#include "JsonConfigOpener.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <Source/Ui/TreeItems/OnexNSipData.h>
#include <Source/Ui/TreeItems/OnexNS4BbData.h>
#include <Source/Ui/TreeItems/OnexNStcData.h>
#include <Source/Ui/TreeItems/OnexNStpData.h>
#include <Source/Ui/TreeItems/OnexNSmpData.h>
#include <Source/Ui/TreeItems/OnexNSmcData.h>
#include <Source/Ui/TreeItems/OnexNStgData.h>

OnexTreeItem *JsonConfigOpener::load(QFile &file, const QString &directory) {
    QJsonObject jo = QJsonDocument::fromJson(file.readAll()).object();
    int headerNumber = getNTHeaderNumber(QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()));
    if (headerNumber == -1) {
        QMessageBox::critical(nullptr, "Not supported!", "This feature is not available for this archive");
        return nullptr;
    }

    OnexTreeItem *root = generateRoot(headerNumber, jo);

    QJsonArray contentArray = jo["content"].toArray();
    for (auto &&i : contentArray) {
        OnexTreeItem *item = generateItem(headerNumber, i.toObject(), directory);
        root->addChild(item);
    }

    return root;
}

OnexTreeItem *JsonConfigOpener::generateRoot(int headerNumber, const QJsonObject &jo) {
    switch (headerNumber) {
        case NSipData:
            return new OnexNSipData(jo["Archive"].toString(), QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()), &zlibOpener);
        case NS4BbData:
            return new OnexNS4BbData(jo["Archive"].toString(), QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()), &zlibOpener);
        case NStcData:
            return new OnexNStcData(jo["Archive"].toString(), QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()), &zlibOpener);
        case NStpData:
        case NStpeData:
        case NStpuData:
            return new OnexNStpData(jo["Archive"].toString(), QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()), &zlibOpener);
        case NSmpData:
        case NSppData:
            return new OnexNSmpData(jo["Archive"].toString(), QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()), &zlibOpener);
        case NStgData:
        case NStgeData:
            return new OnexNStgData(jo["Archive"].toString(), QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()), &zlibOpener);
        case NSmcData:
        case NSpcData:
            return new OnexNSmcData(jo["Archive"].toString(), QByteArray::fromHex(jo["Header"].toString().toLocal8Bit()), &zlibOpener);
        case 199:
            return new OnexTreeText(jo["Archive"].toString(), &textOpener, jo["Last Edit"].toString());
        default:
            return nullptr;
    }
}

OnexTreeItem *JsonConfigOpener::generateItem(int headerNumber, const QJsonObject &jo, const QString &directory) {
    switch (headerNumber) {
        case NSipData:
            return new OnexNSipData(jo, &zlibOpener, directory);
        case NS4BbData:
            return new OnexNS4BbData(jo, &zlibOpener, directory);
        case NStcData:
            return new OnexNStcData(jo, &zlibOpener, directory);
        case NStpData:
        case NStpeData:
        case NStpuData:
            return new OnexNStpData(jo, &zlibOpener, directory);
        case NSmpData:
        case NSppData:
            return new OnexNSmpData(jo, &zlibOpener, directory);
        case NStgData:
        case NStgeData:
            return new OnexNStgData(jo, &zlibOpener, directory);
        case NSmcData:
        case NSpcData:
            return new OnexNSmcData(jo, &zlibOpener, directory);
        case 199:
            return new OnexTreeText(jo, &textOpener, directory);
        default:
            return nullptr;
    }
}

int JsonConfigOpener::getNTHeaderNumber(const QString &header) {
    if (header.mid(0, 7) == "NT Data")
        return header.mid(8, 2).toInt();
    else if (header.mid(0, 10) == "32GBS V1.0")
        return NS4BbData;
    else if (header.mid(0, 10) == "ITEMS V1.0")
        return NSipData2006;
    else if (header.isEmpty())
        return 199;
    else
        return -1;
}