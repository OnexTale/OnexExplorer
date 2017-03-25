#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H
#include <QTreeWidgetItem>

class OnexTreeItem : public QTreeWidgetItem
{
private:
    QByteArray content;
    QString name;
public:
    OnexTreeItem(QString name, QByteArray content = QByteArray());
    bool hasSingleContent();
    QByteArray getContent();
};

#endif // ONEXTREEITEM_H
