#ifndef ONEXNS4BBDATA_H
#define ONEXNS4BBDATA_H
#include "OnexTreeImage.h"


class OnexNS4BbData : public OnexTreeImage
{
public:
    OnexNS4BbData(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;

    virtual ~OnexNS4BbData();

private slots:
    virtual void onReplace() override;
};

#endif // ONEXNS4BBDATA_H
