#ifndef ONEXNSTCDATA_H
#define ONEXNSTCDATA_H
#include "OnexTreeImage.h"

class OnexNStcData : public OnexTreeImage {
    public:
    OnexNStcData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;

    virtual ~OnexNStcData();

    private slots:
    virtual int onReplace(QString directory) override;
};

#endif // ONEXNSTCDATA_H
