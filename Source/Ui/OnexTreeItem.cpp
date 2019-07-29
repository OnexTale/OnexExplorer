#include "OnexTreeItem.h"

OnexTreeItem::OnexTreeItem(QString name, INosFileOpener *opener, QByteArray content) : name(name), opener(opener), content(content) {
    this->setText(0, name);
}

QString OnexTreeItem::getSaveDirectory(QString name, QString filter) {
    return QFileDialog::getSaveFileName(0, tr("Save as..."), name, filter);
}

QByteArray OnexTreeItem::getContent() {
    return content;
}

bool OnexTreeItem::hasParent() {
    return QTreeWidgetItem::parent();
}

short OnexTreeItem::fromLittleEndianToShort(QByteArray array) {
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

int OnexTreeItem::fromLittleEndianToInt(QByteArray array) {
    return qFromLittleEndian<qint32>(reinterpret_cast<const uchar *>(array.data()));
}

QByteArray OnexTreeItem::fromShortToLittleEndian(short number) {
    QByteArray writeArray;
    writeArray.resize(2);
    qToLittleEndian<qint16>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray OnexTreeItem::fromIntToLittleEndian(int number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<qint32>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

int OnexTreeItem::getContentSize() {
    return content.size();
}

QString OnexTreeItem::getName() {
    return name;
}

void OnexTreeItem::setName(QString name) {
    this->name = name;
}

void OnexTreeItem::setContent(QByteArray content) {
    this->content = content;
}

OnexTreeItem::~OnexTreeItem() {
}

int OnexTreeItem::onExport(QString directory) {
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
    return 0;
}

int OnexTreeItem::onExportRaw(QString directory) {
    QString fileName = directory + this->getName() + ".bin";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    if (file.write(this->getContent()) == -1) {
        return 0;
    }
    file.close();
    return 1;
}

int OnexTreeItem::onExportAsOriginal() {
    QString fileName = getSaveDirectory(this->getName(), "NOS Archive (*.NOS)");

    if (fileName.isEmpty())
        return 0;

    if (!fileName.endsWith(".NOS"))
        fileName += ".NOS";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(NULL, "Woops", "Couldn't open this file for writing");
        return 0;
    }

    if (file.write(opener->encrypt(this)) == -1) {
        QMessageBox::critical(NULL, "Woops", "Couldn't write to this file");
        return 0;
    }

    file.close();
    QMessageBox::information(NULL, "Yeah", "File exported into .NOS");

    return 1;
}

int OnexTreeItem::onReplace(QString directory) {
    if (this->childCount() > 0) {
        int count = 0;
        for (int i = 0; i < this->childCount(); i++) {
            OnexTreeItem *item = static_cast<OnexTreeItem *>(this->child(i));
            count += item->onReplace(directory);
        }
        return count;
    } else {
        QString fileName = directory + this->getName();
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
            this->content = file.readAll();
        else
            return 0;
        return 1;
    }
}

int OnexTreeItem::onReplaceRaw(QString directory) {
    QString fileName = directory + this->getName() + ".bin";
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly)) {
        content = file.readAll();
        file.close();
        return 1;
    }
    return 0;
}

void OnexTreeItem::onDelete() {
    delete this;
}

void OnexTreeItem::actionClose() {
    QList<QTreeWidgetItem *> selectedItems = this->treeWidget()->selectedItems();

    foreach (auto &item, selectedItems)
        delete item;
}