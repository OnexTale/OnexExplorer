#include "OnexTreeText.h"
#include "../Openers/NosTextOpener.h"
#include "SingleTextFilePreview.h"

OnexTreeText::OnexTreeText(QString name, NosTextOpener *opener, int fileNumber, int isDat, QByteArray content)
    : OnexTreeItem(name, content), opener(opener), fileNumber(fileNumber), isDat(isDat) {
}

int OnexTreeText::getFileNumber() const {
    return fileNumber;
}

int OnexTreeText::getIsDat() const {
    return isDat;
}

void OnexTreeText::setFileNumber(int fileNumber) {
    this->fileNumber = fileNumber;
}

void OnexTreeText::setIsDat(bool isDat) {
    this->isDat = isDat;
}

QWidget *OnexTreeText::getPreview() {
    if (childCount() != 0)
        return nullptr;
    SingleTextFilePreview *textPreview = new SingleTextFilePreview(content);

    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));

    return textPreview;
}

QWidget *OnexTreeText::getInfos() {
    return nullptr;
}

int OnexTreeText::onExporAsOriginal() {
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

int OnexTreeText::onExport(QString directory) {
    if (childCount() > 0) {
        int count = 0;
        for (int i = 0; i != this->childCount(); ++i) {
            OnexTreeText *item = static_cast<OnexTreeText *>(this->child(i));
            count += item->onExport(directory);
        }
        return count;
    } else {
        QString fileName = directory + this->getName();
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(content);
            file.close();
            return 1;
        }
    }
    return 0;
}

int OnexTreeText::onReplace(QString directory) {
    int count = OnexTreeItem::onReplace(directory);
    emit replaceSignal(this->getContent());
    return count;
}

OnexTreeText::~OnexTreeText() {
}
