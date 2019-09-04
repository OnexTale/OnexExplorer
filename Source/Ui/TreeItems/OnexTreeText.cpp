#include "OnexTreeText.h"
#include "../../Openers/NosTextOpener.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

OnexTreeText::OnexTreeText(const QString &name, NosTextOpener *opener, int fileNumber, int isCompressed, QByteArray content)
        : OnexTreeItem(name, opener, content), fileNumber(fileNumber), isCompressed(isCompressed) {
}

OnexTreeText::OnexTreeText(const QString &name, NosTextOpener *opener, const QString &time) : OnexTreeItem(name, opener, QByteArray()), fileNumber(0),
                                                                                              isCompressed(0), time(time) {
}

OnexTreeText::OnexTreeText(QJsonObject jo, NosTextOpener *opener, const QString &directory) : OnexTreeItem(jo["Name"].toString(), opener) {
    setFileNumber(jo["Filenumber"].toInt(), true);
    setIsCompressed(jo["isCompressed"].toBool());
    onReplace(directory + jo["path"].toString());
}

OnexTreeText::~OnexTreeText() = default;

QWidget *OnexTreeText::getPreview() {
    if (!hasParent())
        return nullptr;
    auto *textPreview = new SingleTextFilePreview(content, getEncoding());
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

int OnexTreeText::getIsCompressed() const {
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

void OnexTreeText::setIsCompressed(bool isCompressed, bool update) {
    this->isCompressed = isCompressed;
    if (update)
            emit changeSignal("isCompressed", isCompressed);
}

void OnexTreeText::setTime(QString time, bool update) {
    this->time = time;
    if (update)
            emit changeSignal("Last Edit", time);
}

FileInfo *OnexTreeText::generateInfos() {
    FileInfo *infos = OnexTreeItem::generateInfos();
    if (!hasParent()) {
        infos->addStringLineEdit("Last Edit", time)->setEnabled(false);
    } else {
        infos = OnexTreeItem::generateInfos();
        connect(infos->addStringLineEdit("Name", text(0)), &QLineEdit::textChanged,
                [=](const QString &value) { setText(0, value); });
        connect(infos->addIntLineEdit("Filenumber", getFileNumber()), &QLineEdit::textChanged,
                [=](const QString &value) { setFileNumber(value.toInt()); });
        connect(infos->addCheckBox("isCompressed", getIsCompressed()), &QCheckBox::clicked, [=](const bool value) { setIsCompressed(value); });
    }
    connect(this, SIGNAL(changeSignal(QString, QString)), infos, SLOT(update(QString, QString)));
    connect(this, SIGNAL(changeSignal(QString, int)), infos, SLOT(update(QString, int)));
    connect(this, SIGNAL(changeSignal(QString, float)), infos, SLOT(update(QString, float)));
    connect(this, SIGNAL(changeSignal(QString, bool)), infos, SLOT(update(QString, bool)));
    return infos;
}

QString OnexTreeText::getEncoding() {
    QRegExp rx = QRegExp("^_code_\\w{2}_\\w*\\.txt$");
    int match = rx.indexIn(getName());
    if (match == -1)
        return "EUC-KR";
    QString region = getName().mid(6, 2);
    if (region == "de" || region == "pl" || region == "it" || region == "cz")
        return "Windows-1250";
    else if (region == "ru")
        return "Windows-1251";
    else if (region == "en" || region == "fr" || region == "es")
        return "Windows-1252";
    else if (region == "tr")
        return "Windows-1254";
}
