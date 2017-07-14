#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H
#include <QTreeWidgetItem>
#include <memory>
#include "../NosEnumTypes.h"

class OnexTreeItem : public QTreeWidgetItem, public std::enable_shared_from_this<OnexTreeItem>
{
private:
    QByteArray content;
    QString name;
public:
    OnexTreeItem(QString name, QByteArray content = QByteArray());
    QByteArray getContent();
    bool hasParent();

    virtual QString getName();
    virtual QWidget* onClicked() = 0;
};

#endif // ONEXTREEITEM_H
