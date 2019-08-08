#include "OnexTreeZlibItem.h"

OnexTreeZlibItem::OnexTreeZlibItem(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                                   int creationDate, bool compressed)
    : OnexTreeItem(name, opener, content), header(header), id(id), creationDate(creationDate), compressed(compressed) {
}

QWidget *OnexTreeZlibItem::getPreview() {
    return nullptr;
}

FileInfo *OnexTreeZlibItem::getInfos() {
    if (!hasParent())
        return nullptr;

    FileInfo *infos = new FileInfo();

    connect(infos->addIntLineEdit("ID", getId()), &QLineEdit::textChanged,
            [=](const QString &value) { setId(value.toInt()); });
    infos->addIntLineEdit("Size", getContentSize())->setEnabled(false);
    infos->addStringLineEdit("Header", getHeader())->setEnabled(false);
    connect(infos->addStringLineEdit("Date", getDateAsString()), &QLineEdit::textChanged,
            [=](const QString &value) { setCreationDate(value); });
    connect(infos->addCheckBox("isCompressed", isCompressed()), &QCheckBox::clicked,
            [=](const bool value) { setCompressed(value); });

    connect(this, SIGNAL(changeSignal(QString, QString)), infos, SLOT(update(QString, QString)));
    connect(this, SIGNAL(changeSignal(QString, int)), infos, SLOT(update(QString, int)));
    connect(this, SIGNAL(changeSignal(QString, float)), infos, SLOT(update(QString, float)));
    connect(this, SIGNAL(changeSignal(QString, bool)), infos, SLOT(update(QString, bool)));
    connect(this, SIGNAL(replaceInfo(FileInfo *)), infos, SLOT(replace(FileInfo *)));

    return infos;
}

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

QByteArray OnexTreeZlibItem::getHeader() {
    return header;
}

void OnexTreeZlibItem::setId(int id, bool update) {
    this->id = id;
    if (update)
        emit changeSignal("ID", id);
}

void OnexTreeZlibItem::setCreationDate(QString date, bool update) {
    QStringList parts = date.split("/", QString::SplitBehavior::SkipEmptyParts);
    if (parts.size() != 3)
        this->creationDate = 0;
    else {
        int year = parts[0].toInt() << 0x10;
        int month = parts[1].toInt() << 0x08;
        int day = parts[2].toInt();
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

void OnexTreeZlibItem::setHeader(QString header, bool update) {
    this->header = header.toLocal8Bit();
    if (update)
        emit changeSignal("Header", header);
}

OnexTreeZlibItem::~OnexTreeZlibItem() {
}
