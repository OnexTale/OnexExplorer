#include "OnexTreeZlibItem.h"

OnexTreeZlibItem::OnexTreeZlibItem(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeItem(name, content),
    id(id),
    creationDate(creationDate),
    compressed(compressed),
    opener(opener)
{

}

QWidget *OnexTreeZlibItem::onClicked()
{
    return nullptr;
}

OnexTreeZlibItem::~OnexTreeZlibItem()
{

}
