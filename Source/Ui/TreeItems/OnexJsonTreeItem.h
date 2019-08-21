#ifndef ONEXJSONTREEITEM_H
#define ONEXJSONTREEITEM_H
#include "../OnexTreeItem.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


class OnexJsonTreeItem : public OnexTreeItem
{
Q_OBJECT

public:
    OnexJsonTreeItem(QString name, INosFileOpener *opener, QByteArray content = QByteArray());
    ~OnexJsonTreeItem() override;
    QWidget *getPreview() override;
    QString getExportExtension() override;
public slots:
    int onExport(QString directory) override;
    int onReplace(QString directory) override;
    int afterReplace(QByteArray content) override;
signals:
    void replaceSignal(QByteArray text);
protected:
    virtual void loadJson(QJsonArray array) = 0;
};

#endif // ONEXJSONTREEITEM_H