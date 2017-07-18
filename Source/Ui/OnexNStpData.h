#ifndef ONEXNSTPDATA_H
#define ONEXNSTPDATA_H
#include "OnexTreeImage.h"

class OnexNStpData : public OnexTreeImage
{
public:
    OnexNStpData(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;

    virtual ~OnexNStpData();
};

#endif // ONEXNSTPDATA_H
