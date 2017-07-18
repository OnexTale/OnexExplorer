#include "OnexTreeZlibItem.h"

OnexTreeZlibItem::OnexTreeZlibItem(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeItem(name, content),
    opener(opener),
    id(id),
    creationDate(creationDate),
    compressed(compressed)
{

}

QWidget *OnexTreeZlibItem::onClicked()
{
    return nullptr;
}

OnexTreeZlibItem::~OnexTreeZlibItem()
{

}
