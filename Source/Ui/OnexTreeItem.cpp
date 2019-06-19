#include "OnexTreeItem.h"

OnexTreeItem::OnexTreeItem(QString name, QByteArray content) {
    this->name = name;
    this->content = content;
    this->setText(0, name);
}

QString OnexTreeItem::getSelectedDirectory() {
    QString dir = QFileDialog::getExistingDirectory(0, tr("Select Directory"));
    if (dir.isEmpty())
        return dir;

    return dir + "/";
}

QString OnexTreeItem::getSaveDirectory(QString name, QString filter) {
    return QFileDialog::getSaveFileName(0, tr("Save as..."), name, filter);
}

QString OnexTreeItem::getOpenDirectory(QString filter) {
    return QFileDialog::getOpenFileName(0, tr("Open..."), "", filter);
}

QByteArray OnexTreeItem::getContent() { return content; }

bool OnexTreeItem::hasParent() { return QTreeWidgetItem::parent(); }

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

int OnexTreeItem::getContentSize() { return content.size(); }

QString OnexTreeItem::getName() { return name; }

OnexTreeItem::~OnexTreeItem() {}

int OnexTreeItem::onExportAll(QString directory) {
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
    return 0;
}

int OnexTreeItem::onExportSingle(QString directory) {
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
    return 0;
}

int OnexTreeItem::onExporSingleRaw(QString directory) {
    QString fileName = directory + this->getName();

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    if (file.write(this->getContent()) == -1) {
        QMessageBox::critical(NULL, tr("Woops"), tr("Couldn't save that file"));
        return 0;
    }
    file.close();
    return 1;
}

int OnexTreeItem::onExporAsOriginal() {
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
    return 0;
}

int OnexTreeItem::onReplace(QString directory) {
    QString fileName = directory + this->getName();
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
        this->content = file.readAll();
    return 1;
}

void OnexTreeItem::onDelete() { delete this; }

void OnexTreeItem::actionClose() {
    QList<QTreeWidgetItem *> selectedItems = this->treeWidget()->selectedItems();

    foreach (auto &item, selectedItems)
        delete item;
}