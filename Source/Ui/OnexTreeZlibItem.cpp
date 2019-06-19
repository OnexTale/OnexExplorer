#include "OnexTreeZlibItem.h"

OnexTreeZlibItem::OnexTreeZlibItem(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                                   bool compressed)
    : OnexTreeItem(name, content), id(id), creationDate(creationDate), compressed(compressed), opener(opener) {}

QWidget *OnexTreeZlibItem::onClicked() { return nullptr; }

int OnexTreeZlibItem::onExporAsOriginal() {
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

    QMessageBox::information(NULL, "Yeah", "File exported into .NOS");

    return 1;
}

int OnexTreeZlibItem::getId() { return id; }

int OnexTreeZlibItem::getCreationDate() { return creationDate; }

bool OnexTreeZlibItem::isCompressed() { return compressed; }

OnexTreeZlibItem::~OnexTreeZlibItem() {}
