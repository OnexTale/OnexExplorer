#include "OnexJsonTreeItem.h"

OnexJsonTreeItem::OnexJsonTreeItem(QString name, INosFileOpener *opener, QByteArray content)
        : OnexTreeItem(name, opener, content) {
}

OnexJsonTreeItem::~OnexJsonTreeItem() = default;

QWidget *OnexJsonTreeItem::getPreview() {
    if (!hasParent())
        return nullptr;
    auto *textPreview = new SingleTextFilePreview(content);

    connect(this, SIGNAL(replaceSignal(QByteArray)), textPreview, SLOT(onReplaced(QByteArray)));

    return textPreview;
}

QString OnexJsonTreeItem::getExportExtension() {
    return ".json";
}

int OnexJsonTreeItem::onExport(QString directory) {
    if (!hasParent()) {
        QMessageBox::StandardButton message = QMessageBox::question(nullptr, "Export?", "Export all items together as one JSON?");
        if (message == QMessageBox::Yes) {
            QJsonArray fullJson;
            for (int i = 0; i < childCount(); i++) {
                auto *ct = static_cast<OnexJsonTreeItem *>(child(i));
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

int OnexJsonTreeItem::onReplace(QString directory) {
    if (!hasParent()) {
        QMessageBox::StandardButton message = QMessageBox::question(nullptr, "Replace?", "Replace all items together from one JSON?");
        if (message == QMessageBox::Yes) {
            takeChildren().clear();
            QString path = directory + name + ".json";
            QFile file(path);
            if (file.open(QIODevice::ReadOnly)) {
                QJsonArray fullJson = QJsonDocument::fromJson(file.readAll()).array();
                file.close();
                loadJson(fullJson);
                return childCount();
            } else {
                QMessageBox::critical(nullptr, "Woops", "Couldn't open " + path);
                return 0;
            }
        } else {
            return OnexTreeItem::onExport(directory);
        }
    }
    return OnexTreeItem::onExport(directory);
}

int OnexJsonTreeItem::afterReplace(QByteArray content) {
    setContent(content);
    emit replaceSignal(this->getContent());
    return 1;
}