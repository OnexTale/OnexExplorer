#include "OnexTreeItem.h"

OnexTreeItem::OnexTreeItem(QString name, NosEnumTypes type, QByteArray content, int headerValue, int timestamp)
{
    this->content = content;
    this->name = name;
    this->type = type;
    this->headerValue = headerValue;
    this->timestamp = timestamp;
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

NosEnumTypes OnexTreeItem::getType()
{
    return type;
}

int OnexTreeItem::getHeaderValue()
{
    return headerValue;
}

QString OnexTreeItem::getName()
{
    return name;
}

int OnexTreeItem::getTimestamp()
{
    return timestamp;
}
