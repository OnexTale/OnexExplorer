#ifndef ONEXNSMPDATA_H
#define ONEXNSMPDATA_H
#include "../Openers/NosZlibOpener.h"
#include "OnexTreeZlibItem.h"

class OnexNSmpData : public OnexTreeZlibItem {
public:
    OnexNSmpData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    virtual QByteArray getContent() override;
    virtual ~OnexNSmpData();
    virtual QWidget *onClicked() override;

private slots:
    virtual int onExport(QString directory);
    virtual int onReplace(QString directory);
};

#endif // ONEXNSMPDATA_H
