#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H
#include <QTreeWidgetItem>
#include <QtEndian>

class OnexTreeItem : public QTreeWidgetItem
{
protected:
    QByteArray content;
    QString name;
public:
    OnexTreeItem(QString name, QByteArray content = QByteArray());
    QByteArray getContent();
    bool hasParent();
    short fromLittleEndianToShort(QByteArray array);

    virtual QString getName();
    virtual QWidget *onClicked() = 0;
    virtual ~OnexTreeItem();
};

#endif // ONEXTREEITEM_H
