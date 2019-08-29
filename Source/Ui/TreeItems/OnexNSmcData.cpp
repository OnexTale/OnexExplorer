#include "OnexNSmcData.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

OnexNSmcData::OnexNSmcData(const QString &name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed) : OnexTreeZlibItem(name, content, opener, id, creationDate, compressed) {
}

OnexNSmcData::~OnexNSmcData() = default;

QWidget *OnexNSmcData::getPreview() {
    if (!hasParent())
        return nullptr;
    QByteArray json = QJsonDocument(toJson()).toJson();
    auto *textPreview = new SingleTextFilePreview(json);
    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));
    return textPreview;
}

QString OnexNSmcData::getExportExtension() {
    return ".json";
}

int OnexNSmcData::saveAsFile(const QString &path, QByteArray content) {
    return OnexTreeItem::saveAsFile(path, QJsonDocument(toJson()).toJson());
}

int OnexNSmcData::afterReplace(QByteArray content) {
    setContent(fromJson(content));
    emit replaceSignal(this->getContent());
    return 1;
}

QJsonObject OnexNSmcData::toJson() {
    QJsonObject jo;
    int amount = content.at(0);
    uint8_t flag = content.at(1);
    jo["loop"] = (bool) (flag >> 7);
    jo["unknown"] = (bool) ((flag >> 6) & 1);
    int offset = 2;

    QJsonArray array;
    for (int i = 0; i < amount; i++) {
        QJsonObject frame;
        frame["spriteIndex"] = content.at(offset);
        frame["triggerEffect"] = content.at(offset + 1) == 9; // 0=false, 9=true
        array.append(frame);
        offset += 2;
    }
    jo["frames"] = array;

    return jo;
}

QByteArray OnexNSmcData::fromJson(const QByteArray &data) {
    QJsonObject jo = QJsonDocument::fromJson(data).object();
    QJsonArray array = jo["frames"].toArray();
    QByteArray newContent;
    newContent.append((uint8_t) array.size());
    bool loop = jo["loop"].toBool();
    bool unknown = jo["unknown"].toBool();
    uint8_t flag = (loop << 7) + (unknown << 6);
    newContent.append(flag);
    for (QJsonValueRef value : array) {
        QJsonObject frame = value.toObject();
        newContent.append((uint8_t) frame["spriteIndex"].toInt());
        newContent.append((uint8_t) (frame["triggerEffect"].toBool() ? 9 : 0));
    }
    return newContent;
}