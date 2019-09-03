#include "OnexTreeZlibItem.h"
#include <QDate>

OnexTreeZlibItem::OnexTreeZlibItem(const QString &name, NosZlibOpener *opener, QByteArray content, int id, int creationDate, bool compressed)
        : OnexTreeItem(name, opener, content), id(id), creationDate(creationDate),
          compressed(compressed) {
    if (creationDate == 0)
        setCreationDate(QDate::currentDate().toString("dd/MM/yyyy"), true);
}


OnexTreeZlibItem::~OnexTreeZlibItem() = default;

int OnexTreeZlibItem::getId() {
    return id;
}

int OnexTreeZlibItem::getCreationDate() {
    return creationDate;
}

QString OnexTreeZlibItem::getDateAsString() {
    int year = (getCreationDate() & 0xFFFF0000) >> 0x10;
    int month = (getCreationDate() & 0xFF00) >> 0x08;
    int day = getCreationDate() & 0xFF;
    return QString("%1/%2/%3").arg(day, 2, 16, QChar('0')).arg(month, 2, 16, QChar('0')).arg(year, 4, 16, QChar('0'));
}

bool OnexTreeZlibItem::isCompressed() {
    return compressed;
}

void OnexTreeZlibItem::setName(QString name) {
    OnexTreeItem::setName(name);
    setId(name.toInt(), true);
}

void OnexTreeZlibItem::setId(int id, bool update) {
    this->id = id;
    OnexTreeItem::setName(QString::number(id));
    if (update)
            emit changeSignal("ID", id);
}

void OnexTreeZlibItem::setCreationDate(const QString &date, bool update) {
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

void OnexTreeZlibItem::setCompressed(bool compressed, bool update) {
    this->compressed = compressed;
    if (update)
            emit changeSignal("isCompressed", compressed);
}

FileInfo *OnexTreeZlibItem::generateInfos() {
    auto *infos = OnexTreeItem::generateInfos();
    if (getId() == -1) {
        infos->addStringLineEdit("Header", QString::fromUtf8(getContent(), getContent().size()))->setEnabled(false);
    } else {
        connect(infos->addIntLineEdit("ID", getId()), &QLineEdit::textChanged,
                [=](const QString &value) { setId(value.toInt()); });
        connect(infos->addStringLineEdit("Date", getDateAsString()), &QLineEdit::textChanged,
                [=](const QString &value) { setCreationDate(value); });
        connect(infos->addCheckBox("isCompressed", isCompressed()), &QCheckBox::clicked,
                [=](const bool value) { setCompressed(value); });
    }
    return infos;
}
