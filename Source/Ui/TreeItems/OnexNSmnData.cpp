#include "OnexNSmnData.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

OnexNSmnData::OnexNSmnData(const QString &name, int creationDate, INosFileOpener *opener, QByteArray content) : OnexTreeItem(name, opener, content),
                                                                                                                creationDate(creationDate) {

}

int OnexNSmnData::getCreationDate() {
    return creationDate;
}

QString OnexNSmnData::getDateAsString() {
    int year = (getCreationDate() & 0xFFFF0000) >> 0x10;
    int month = (getCreationDate() & 0xFF00) >> 0x08;
    int day = getCreationDate() & 0xFF;
    return QString("%1/%2/%3").arg(day, 2, 16, QChar('0')).arg(month, 2, 16, QChar('0')).arg(year, 4, 16, QChar('0'));
}

void OnexNSmnData::loadJson(QJsonArray array) {
    for (auto &&i : array) {
        QJsonObject jo = i.toObject();
        addChild(new OnexNSmnData(QString::number(childCount()), creationDate, opener, QJsonDocument(jo).toJson()));
    }
}

void OnexNSmnData::setCreationDate(const QString &date, bool update) {
    QStringList parts = date.split("/", QString::SplitBehavior::SkipEmptyParts);
    if (parts.size() != 3)
        this->creationDate = 0;
    else {
        int year = parts[2].toInt(nullptr, 16) << 0x10;
        int month = parts[1].toInt(nullptr, 16) << 0x08;
        int day = parts[0].toInt(nullptr, 16);
        this->creationDate = year + month + day;
    }
    if (update)
            emit changeSignal("Date", getDateAsString());
}

FileInfo *OnexNSmnData::generateInfos() {
    auto *infos = OnexTreeItem::generateInfos();
    if (!hasParent()) {
        connect(infos->addStringLineEdit("Header", getContent()), &QLineEdit::textChanged,
                [=](const QString &value) { setContent(value.toLocal8Bit()); });
        connect(infos->addStringLineEdit("Date", getDateAsString()), &QLineEdit::textChanged,
                [=](const QString &value) { setCreationDate(value); });
    }
    return infos;
}

QWidget *OnexNSmnData::getPreview() {
    if (!hasParent())
        return nullptr;
    auto *textPreview = new SingleTextFilePreview(content);
    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));
    return textPreview;
}

QString OnexNSmnData::getExportExtension() {
    return ".json";
}

int OnexNSmnData::onExport(QString directory) {
    if (!hasParent()) {
        QMessageBox::StandardButton message = QMessageBox::question(nullptr, "Export?", "Export all items together as one JSON?");
        if (message == QMessageBox::Yes) {
            QJsonArray fullJson;
            for (int i = 0; i < childCount(); i++) {
                auto *ct = static_cast<OnexNSmnData *>(child(i));
                QJsonObject jo = QJsonDocument::fromJson(ct->getContent()).object();
                fullJson.append(jo);
            }
            saveAsFile(directory + name + ".json", QJsonDocument(fullJson).toJson());
            return 1;
        } else {
            return OnexTreeItem::onExport(directory);
        }
    }
    return OnexTreeItem::onExport(directory);
}

int OnexNSmnData::onReplace(QString directory) {
    if (!hasParent()) {
        QMessageBox::StandardButton message = QMessageBox::question(nullptr, "Replace?", "Replace all items together from one JSON?");
        if (message == QMessageBox::Yes) {
            QString path = directory + name + ".json";
            QFile file(path);
            if (file.open(QIODevice::ReadOnly)) {
                QJsonArray fullJson = QJsonDocument::fromJson(file.readAll()).array();
                file.close();
                takeChildren().clear();
                loadJson(fullJson);
                return childCount();
            } else {
                QMessageBox::critical(nullptr, "Woops", "Couldn't open " + path);
                return 0;
            }
        }
    }
    return OnexTreeItem::onReplace(directory);
}

int OnexNSmnData::afterReplace(QByteArray content) {
    setContent(content);
    emit replaceSignal(this->getContent());
    return 1;
}