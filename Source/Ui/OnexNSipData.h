#ifndef ONEXNSIPDATA_H
#define ONEXNSIPDATA_H
#include "OnexTreeImage.h"

class OnexNSipData : public OnexTreeImage
{
private:

public:
    OnexNSipData(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;
    virtual ~OnexNSipData();

private slots:
    virtual void onReplace() override;


};

#endif // ONEXNSIPDATA_H
