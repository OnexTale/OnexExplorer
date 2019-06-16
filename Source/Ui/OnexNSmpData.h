#ifndef ONEXNSMPDATA_H
#define ONEXNSMPDATA_H
#include "OnexTreeItem.h"
#include "OnexTreeZlibItem.h"
#include "../Openers/NosZlibOpener.h"

class OnexNSmpData : public OnexTreeZlibItem
{
private:
    int exportFrames(OnexNSmpData *src, QString path);
public:
    OnexNSmpData(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);
    virtual QByteArray getContent() override;
    virtual ~OnexNSmpData();

private slots:
    virtual void onExportAll();
    virtual void onExportSingle();
    virtual void onReplace();
};

#endif // ONEXNSMPDATA_H
