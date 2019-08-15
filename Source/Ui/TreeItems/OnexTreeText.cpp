#include "OnexTreeText.h"
#include "../../Openers/NosTextOpener.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

OnexTreeText::OnexTreeText(const QString &name, NosTextOpener *opener, int fileNumber, int isDat,
                           QByteArray content)
        : OnexTreeItem(name, opener, content), fileNumber(fileNumber), isDat(isDat) {
}

OnexTreeText::~OnexTreeText() = default;

QWidget *OnexTreeText::getPreview() {
    if (childCount() != 0)
        return nullptr;
    auto *textPreview = new SingleTextFilePreview(content);
    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));
    return textPreview;
}

QString OnexTreeText::getExportExtension() {
    return name.split(".").at(1);
}

int OnexTreeText::getFileNumber() const {
    return fileNumber;
}

int OnexTreeText::getIsDat() const {
    return isDat;
}

int OnexTreeText::afterReplace(QByteArray content) {
    this->setContent(content);
    emit replaceSignal(this->getContent());
    return 1;
}

void OnexTreeText::setFileNumber(int fileNumber, bool update) {
    this->fileNumber = fileNumber;
    if (update)
            emit changeSignal("Filenumber", fileNumber);
}

void OnexTreeText::setIsDat(bool isDat, bool update) {
    this->isDat = isDat;
    if (update)
            emit changeSignal("isDat", isDat);
}

FileInfo *OnexTreeText::generateInfos() {
    auto *infos = new FileInfo();
    connect(infos->addIntLineEdit("Filenumber", getFileNumber()), &QLineEdit::textChanged,
            [=](const QString &value) { setFileNumber(value.toInt()); });
    connect(infos->addCheckBox("isDat", getIsDat()), &QCheckBox::clicked, [=](const bool value) { setIsDat(value); });
    connect(this, SIGNAL(changeSignal(QString, QString)), infos, SLOT(update(QString, QString)));
    connect(this, SIGNAL(changeSignal(QString, int)), infos, SLOT(update(QString, int)));
    connect(this, SIGNAL(changeSignal(QString, float)), infos, SLOT(update(QString, float)));
    connect(this, SIGNAL(changeSignal(QString, bool)), infos, SLOT(update(QString, bool)));
    return infos;
}