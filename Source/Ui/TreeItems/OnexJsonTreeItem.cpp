#include "OnexJsonTreeItem.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

OnexJsonTreeItem::OnexJsonTreeItem(QString name, INosFileOpener *opener, QByteArray content)
        : OnexTreeItem(name, opener, content) {
}

OnexJsonTreeItem::~OnexJsonTreeItem() = default;

QWidget *OnexJsonTreeItem::getPreview() {
    if (childCount() != 0)
        return nullptr;
    auto *textPreview = new SingleTextFilePreview(content);

    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));

    return textPreview;
}

QString OnexJsonTreeItem::getExportExtension() {
    return ".json";
}

int OnexJsonTreeItem::afterReplace(QByteArray content) {
    setContent(content);
    emit replaceSignal(this->getContent());
    return 1;
}