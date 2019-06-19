#ifndef ONEXNSMPDATA_H
#define ONEXNSMPDATA_H
#include "../Openers/NosZlibOpener.h"
#include "OnexTreeItem.h"
#include "OnexTreeZlibItem.h"

class OnexNSmpData : public OnexTreeZlibItem {
    private:
    int exportFrames(OnexNSmpData *src, QString path);
    int replaceFrames(OnexNSmpData *src, QString path);

    public:
    OnexNSmpData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    virtual QByteArray getContent() override;
    virtual ~OnexNSmpData();

    private slots:
    virtual int onExportAll(QString directory);
    virtual int onExportSingle(QString directory);
    virtual int onReplace(QString directory);
};

#endif // ONEXNSMPDATA_H
