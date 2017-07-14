#include "OnexTreeItem.h"

OnexTreeItem::OnexTreeItem(QString name, QByteArray content)
{
    this->content = content;
}

QByteArray OnexTreeItem::getContent()
{
    return content;
}

bool OnexTreeItem::hasParent()
{
    return this->parent();
}

QString OnexTreeItem::getName()
{
    return name;
}
