#ifndef ONEXNSMPDATA_H
#define ONEXNSMPDATA_H
#include "OnexTreeItem.h"
#include "OnexTreeZlibItem.h"
#include "../Openers/NosZlibOpener.h"

class OnexNSmpData : public OnexTreeZlibItem
{
public:
    OnexNSmpData(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);
    virtual QWidget *onClicked() override;
    virtual QByteArray getContent() override;
    virtual ~OnexNSmpData();
};

#endif // ONEXNSMPDATA_H
