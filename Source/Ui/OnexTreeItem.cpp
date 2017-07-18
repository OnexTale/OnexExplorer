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

short OnexTreeItem::fromLittleEndianToShort(QByteArray array)
{
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

QString OnexTreeItem::getName()
{
    return name;
}

OnexTreeItem::~OnexTreeItem()
{

}
