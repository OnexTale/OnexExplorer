#ifndef ONEXNSTPDATA_H
#define ONEXNSTPDATA_H
#include "OnexTreeImage.h"

class OnexNStpData : public OnexTreeImage
{
public:
    OnexNStpData(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);

    int getFormat();
    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;

    virtual ~OnexNStpData();

private slots:
    virtual void onReplace() override;
};

#endif // ONEXNSTPDATA_H
