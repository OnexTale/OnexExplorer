#ifndef ONEXJSONTREEITEM_H
#define ONEXJSONTREEITEM_H
#include "../OnexTreeItem.h"
#include "../Previews/SingleTextFilePreview.h"
#include <QJsonObject>


class OnexJsonTreeItem : public OnexTreeItem
{
Q_OBJECT

public:
    OnexJsonTreeItem(QString name, INosFileOpener *opener, QByteArray content = QByteArray());
    ~OnexJsonTreeItem() override;
    QWidget *getPreview() override;
    QString getExportExtension() override;
public slots:
    int afterReplace(QByteArray content) override;
signals:
    void replaceSignal(QByteArray text);
};

#endif // ONEXJSONTREEITEM_H