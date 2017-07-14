#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H
#include <QTreeWidgetItem>
#include <memory>
#include "../NosEnumTypes.h"

class OnexTreeItem : public QTreeWidgetItem
{
protected:
    QByteArray content;
    QString name;
public:
    OnexTreeItem(QString name, QByteArray content = QByteArray());
    QByteArray getContent();
    bool hasParent();

    virtual QString getName();
    virtual QWidget* onClicked() = 0;
    virtual ~OnexTreeItem();
};

#endif // ONEXTREEITEM_H
