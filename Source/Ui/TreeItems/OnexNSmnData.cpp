#include "OnexNSmnData.h"

OnexNSmnData::OnexNSmnData(const QString &name, int creationDate, INosFileOpener *opener, QByteArray content) : OnexJsonTreeItem(name, opener, content),
                                                                                                                creationDate(creationDate) {

}

FileInfo *OnexNSmnData::getInfos() {
    if (!hasParent())
        return generateInfos();
    return nullptr;
}

int OnexNSmnData::getCreationDate() {
    return creationDate;
}

QString OnexNSmnData::getDateAsString() {
    int year = (getCreationDate() & 0xFFFF0000) >> 0x10;
    int month = (getCreationDate() & 0xFF00) >> 0x08;
    int day = getCreationDate() & 0xFF;
    return QString("%1/%2/%3").arg(day, 2, 16, QChar('0')).arg(month, 2, 16, QChar('0')).arg(year, 4, 16, QChar('0'));
}

void OnexNSmnData::loadJson(QJsonArray array) {
    for (auto &&i : array) {
        QJsonObject jo = i.toObject();
        addChild(new OnexNSmnData(QString::number(childCount()), creationDate, opener, QJsonDocument(jo).toJson()));
    }
}

void OnexNSmnData::setCreationDate(const QString &date, bool update) {
    QStringList parts = date.split("/", QString::SplitBehavior::SkipEmptyParts);
    if (parts.size() != 3)
        this->creationDate = 0;
    else {
        int year = parts[2].toInt(nullptr, 16) << 0x10;
        int month = parts[1].toInt(nullptr, 16) << 0x08;
        int day = parts[0].toInt(nullptr, 16);
        this->creationDate = year + month + day;
    }
    if (update)
            emit changeSignal("Date", getDateAsString());
}

FileInfo *OnexNSmnData::generateInfos() {
    auto *infos = OnexTreeItem::generateInfos();
    if (!hasParent()) {
        connect(infos->addStringLineEdit("Header", getContent()), &QLineEdit::textChanged,
                [=](const QString &value) { setContent(value.toLocal8Bit()); });
        connect(infos->addStringLineEdit("Date", getDateAsString()), &QLineEdit::textChanged,
                [=](const QString &value) { setCreationDate(value); });
    }
    return infos;
}
