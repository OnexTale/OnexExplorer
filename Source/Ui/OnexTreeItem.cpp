#include "OnexTreeItem.h"

OnexTreeItem::OnexTreeItem(QString name, QByteArray content)
{
    this->content = content;
    this->name = name;
    this->setText(0, name);
}

bool OnexTreeItem::hasParent()
{
    return this->parent();
}

QByteArray OnexTreeItem::getContent()
{
    return content;
}