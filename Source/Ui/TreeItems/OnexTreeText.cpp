#include "OnexTreeText.h"
#include "../../Openers/NosTextOpener.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

OnexTreeText::OnexTreeText(QString name, NosTextOpener *opener, int fileNumber, int isDat, QByteArray content)
    : OnexTreeItem(name, opener, content), fileNumber(fileNumber), isDat(isDat) {
}

int OnexTreeText::getFileNumber() const {
    return fileNumber;
}

int OnexTreeText::getIsDat() const {
    return isDat;
}

void OnexTreeText::setFileNumber(int fileNumber) {
    this->fileNumber = fileNumber;
    emit changeSignal("Filenum", fileNumber);
}

void OnexTreeText::setIsDat(bool isDat) {
    this->isDat = isDat;
    emit changeSignal("isDat", isDat);
}

QWidget *OnexTreeText::getPreview() {
    if (childCount() != 0)
        return nullptr;
    SingleTextFilePreview *textPreview = new SingleTextFilePreview(content);

    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));

    return textPreview;
}

FileInfo *OnexTreeText::getInfos() {
    if (!hasParent())
        return nullptr;

    FileInfo *infos = new FileInfo();

    connect(infos->addIntLineEdit("Filenumber", getFileNumber()), &QLineEdit::textChanged,
            [=](const QString &value) { setFileNumber(value.toInt()); });
    connect(infos->addCheckBox("isDat", getIsDat()), &QCheckBox::clicked, [=](const bool value) { setIsDat(value); });

    connect(this, SIGNAL(changeSignal(QString, QString)), infos, SLOT(update(QString, QString)));
    connect(this, SIGNAL(changeSignal(QString, int)), infos, SLOT(update(QString, int)));
    connect(this, SIGNAL(changeSignal(QString, float)), infos, SLOT(update(QString, float)));
    connect(this, SIGNAL(changeSignal(QString, bool)), infos, SLOT(update(QString, bool)));

    return infos;
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
