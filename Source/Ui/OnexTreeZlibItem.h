#ifndef ONEXTREEZLIBITEM_H
#define ONEXTREEZLIBITEM_H
#include "OnexTreeItem.h"
#include "../Openers/NosZlibOpener.h"

class OnexTreeZlibItem : public OnexTreeItem
{
protected:
    int id;
    int creationDate;
    bool compressed;
    NosZlibOpener* opener;
public:
    OnexTreeZlibItem(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);
    virtual QWidget *onClicked() override;

    virtual ~OnexTreeZlibItem();
};

#endif // ONEXTREEZLIBITEM_H
