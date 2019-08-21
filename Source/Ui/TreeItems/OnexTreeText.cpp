#include "OnexTreeText.h"
#include "../../Openers/NosTextOpener.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

OnexTreeText::OnexTreeText(const QString &name, NosTextOpener *opener, int fileNumber, int isCompressed,
                           QByteArray content)
        : OnexTreeItem(name, opener, content), fileNumber(fileNumber), isCompressed(isCompressed) {
}

OnexTreeText::~OnexTreeText() = default;

QWidget *OnexTreeText::getPreview() {
    if (!hasParent())
        return nullptr;
    auto *textPreview = new SingleTextFilePreview(content);
    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));
    return textPreview;
}

QString OnexTreeText::getExportExtension() {
    QStringList split = name.split(".", QString::SplitBehavior::SkipEmptyParts);
    if (split.size() > 1)
        return split.at(split.size() - 1);
    return "";
}

int OnexTreeText::getFileNumber() const {
    return fileNumber;
}

int OnexTreeText::getIsDat() const {
    return isCompressed;
}

int OnexTreeText::afterReplace(QByteArray content) {
    this->setContent(content);
    emit replaceSignal(this->getContent());
    emit replaceInfo(generateInfos());
    return 1;
}

void OnexTreeText::setFileNumber(int fileNumber, bool update) {
    this->fileNumber = fileNumber;
    if (update)
            emit changeSignal("Filenumber", fileNumber);
}

void OnexTreeText::setIsDat(bool isCompressed, bool update) {
    this->isCompressed = isCompressed;
    if (update)
            emit changeSignal("isCompressed", isCompressed);
}

FileInfo *OnexTreeText::generateInfos() {
    if (!hasParent())
        return nullptr;
    auto *infos = OnexTreeItem::generateInfos();
    connect(infos->addIntLineEdit("Filenumber", getFileNumber()), &QLineEdit::textChanged,
            [=](const QString &value) { setFileNumber(value.toInt()); });
    connect(infos->addCheckBox("isCompressed", getIsDat()), &QCheckBox::clicked, [=](const bool value) { setIsDat(value); });
    connect(this, SIGNAL(changeSignal(QString, QString)), infos, SLOT(update(QString, QString)));
    connect(this, SIGNAL(changeSignal(QString, int)), infos, SLOT(update(QString, int)));
    connect(this, SIGNAL(changeSignal(QString, float)), infos, SLOT(update(QString, float)));
    connect(this, SIGNAL(changeSignal(QString, bool)), infos, SLOT(update(QString, bool)));
    return infos;
}
