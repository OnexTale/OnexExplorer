#ifndef ONEXNSIPDATA_H
#define ONEXNSIPDATA_H
#include "OnexTreeImage.h"

class OnexNSipData : public OnexTreeImage {
    Q_OBJECT
public:
    OnexNSipData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;
    virtual ~OnexNSipData();

public slots:
    virtual int onReplace(QString directory) override;
    virtual void setWidth(int width, bool update = false);
    virtual void setHeight(int height, bool update = false);
};

#endif // ONEXNSIPDATA_H
