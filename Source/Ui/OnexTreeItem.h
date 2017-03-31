#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H
#include <QTreeWidgetItem>
#include "../NosEnumTypes.h"

class OnexTreeItem : public QTreeWidgetItem
{
private:
    QByteArray content;
    QString name;
    NosEnumTypes type;
    int timestamp;
    int headerValue;
public:
    OnexTreeItem(QString name, NosEnumTypes type, QByteArray content = QByteArray(), int headerValue = -1, int timestamp = -1);
    bool hasParent();
    QByteArray getContent();
    NosEnumTypes getType();
    int getHeaderValue();
};

#endif // ONEXTREEITEM_H
