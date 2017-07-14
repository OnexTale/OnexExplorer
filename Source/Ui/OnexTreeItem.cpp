#include "OnexTreeItem.h"

OnexTreeItem::OnexTreeItem(QString name, QByteArray content)
{
    this->name = name;
    this->content = content;
    this->setText(0, name);
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

OnexTreeItem::~OnexTreeItem()
{

}
