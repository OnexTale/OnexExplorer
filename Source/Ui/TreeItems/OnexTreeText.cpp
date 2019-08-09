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

void OnexTreeText::setFileNumber(int fileNumber, bool update) {
    this->fileNumber = fileNumber;
    if (update)
        emit changeSignal("Filenum", fileNumber);
}

void OnexTreeText::setIsDat(bool isDat, bool update) {
    this->isDat = isDat;
    if (update)
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

    FileInfo *infos = generateInfos();
    connect(this, SIGNAL(replaceInfo(FileInfo *)), infos, SLOT(replace(FileInfo *)));
    return infos;
}

FileInfo *OnexTreeText::generateInfos() {
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
        QString path = "";
        if (!directory.endsWith(getExportExtension()))
            path = directory + this->name;
        else
            path = directory;

        QFile file(path);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(content);
            file.close();
            return 1;
        }
    }
    return 0;
}

int OnexTreeText::onReplace(QString directory) {
    int count = 0;
    if (this->childCount() > 0) {
        for (int i = 0; i < this->childCount(); i++) {
            OnexTreeItem *item = static_cast<OnexTreeItem *>(this->child(i));
            count += item->onReplace(directory);
        }
    } else {
        QString path;
        if (!directory.endsWith(getExportExtension()))
            path = directory + this->getName();
        else
            path = directory;

        QFile file(path);
        if (file.open(QIODevice::ReadOnly)) {
            this->content = file.readAll();
            count = 1;
        }
        else {
            QMessageBox::critical(NULL, "Woops", "Couldn't open " + path);
            return 0;
        }
    }

    emit replaceSignal(this->getContent());
    return count;
}

QString OnexTreeText::getExportExtension() {
    return name.split(".").at(1);
}

OnexTreeText::~OnexTreeText() {
}
