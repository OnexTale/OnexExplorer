#ifndef ONEXNSMPDATA_H
#define ONEXNSMPDATA_H
#include "OnexTreeImage.h"

class OnexNSmpData : public OnexTreeImage
{
public:
    OnexNSmpData(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);

    virtual QImage getImage();
    virtual ImageResolution getResolution();
    virtual ~OnexNSmpData();
};

#endif // ONEXNSMPDATA_H
