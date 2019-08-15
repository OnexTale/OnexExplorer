#include "NosCCInfOpener.h"
#include "../Ui/TreeItems/OnexNSmnData.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

NosCCInfOpener::NosCCInfOpener() = default;

OnexTreeItem *NosCCInfOpener::decrypt(QFile &file) {
    file.seek(0);

    QByteArray header = file.read(0xC);
    int creationDate = littleEndianConverter.readInt(file);

    OnexNSmnData *item = new OnexNSmnData(header, neatFileName(file.fileName()), creationDate, this);
    bool playerInfos = item->getName().contains("NSpn");

    int fileSize = littleEndianConverter.readInt(file);
    littleEndianConverter.readInt(file); // second time the same bytes, idk why
    file.read(1);      // 0x00
    int fileAmount = littleEndianConverter.readInt(file);

    for (int i = 0; i < fileAmount; i++) {

        QJsonObject jo;
        jo["direction"] = file.read(1).at(0);
        jo["animation"] = file.read(1).at(0);

        if (playerInfos) {
            uint8_t classSex = (file.read(1).at(0));
            jo["sex"] = (uint8_t) ((classSex & 0x80) != 0);
            jo["class"] = classSex & 0x0F;
            jo["morph"] = file.read(1).at(0);
        } else {
            jo["monster"] = littleEndianConverter.readShort(file);
        }

        jo["base"] = littleEndianConverter.readInt(file);
        jo["nspm"] = littleEndianConverter.readInt(file);
        jo["kit"] = littleEndianConverter.readInt(file);

        QJsonObject parts;
        for (int j = 0; j < 7; j++) {
            QString textureName = QString::number(j);
            if (playerInfos) {
                textureName = getTextureName(j);
            }

            int count = file.read(1).at(0);
            QJsonArray sprites;
            for (int c = 0; c < count; c++) {
                QJsonObject sprite;
                sprite["index"] = littleEndianConverter.readShort(file);
                sprite["id"] = littleEndianConverter.readInt(file);
                sprites.append(sprite);
            }
            parts.insert(textureName, sprites);
        }
        jo.insert("parts", parts);

        QByteArray array = QJsonDocument(jo).toJson();
        item->addChild(new OnexNSmnData(header, QString::number(i), creationDate, this, array));
    }
    return item;
}

QByteArray NosCCInfOpener::encrypt(OnexTreeItem *item) {
    bool playerInfos = item->getName().contains("NSpn");
    OnexNSmnData *nsmnItem = static_cast<OnexNSmnData *>(item);

    QByteArray content;
    QByteArray head;

    content.append(littleEndianConverter.toInt(item->childCount()));
    for (int i = 0; i != item->childCount(); ++i) {
        OnexNSmnData *ct = static_cast<OnexNSmnData *>(item->child(i));
        QJsonObject jo = QJsonDocument::fromJson(ct->getContent()).object();
        content.append((uint8_t) jo["direction"].toInt());
        content.append((uint8_t) jo["animation"].toInt());
        if (playerInfos) {
            int sex = jo["sex"].toInt() == 0 ? 0 : 0x80;
            int cClass = jo["class"].toInt();
            uint8_t classSex = sex + cClass;
            content.append((uint8_t) classSex);
            content.append((uint8_t) jo["morph"].toInt());
        } else {
            content.append(littleEndianConverter.toShort(jo["monster"].toInt()));
        }

        content.append(littleEndianConverter.toInt(jo["base"].toInt()));
        content.append(littleEndianConverter.toInt(jo["nspm"].toInt()));
        content.append(littleEndianConverter.toInt(jo["kit"].toInt()));

        QJsonObject parts = jo["parts"].toObject();

        for (int j = 0; j < 7; j++) {
            QString textureName = QString::number(j);
            if (playerInfos) {
                textureName = getTextureName(j);
            }

            QJsonArray sprites = parts[textureName].toArray();
            content.append((uint8_t) sprites.size());
            for (QJsonValueRef sprite : sprites) {
                content.append(littleEndianConverter.toShort(sprite.toObject()["index"].toInt()));
                content.append(littleEndianConverter.toInt(sprite.toObject()["id"].toInt()));
            }
        }
    }

    head.append(nsmnItem->getHeader());
    head.append(littleEndianConverter.toInt(nsmnItem->getCreationDate()));
    head.append(littleEndianConverter.toInt(content.size())); // fileSize
    head.append(littleEndianConverter.toInt(content.size())); // fileSize
    head.append((char) 0x0);

    content = head.append(content);

    return content;
}

QString NosCCInfOpener::getTextureName(int index) {
    switch (index) {
        case 0:
            return "armor";
        case 1:
            return "hair";
        case 2:
            return "hat";
        case 3:
            return "mask";
        case 4:
            return "secondary-Weapon";
        case 5:
            return "first-Weapon";
        case 6:
            return "weapon-part";
        default:
            return QString::number(index);
    }
}
