#ifndef ONEXNSTCDATA_H
#define ONEXNSTCDATA_H
#include "OnexTreeImage.h"

class OnexNStcData : public OnexTreeImage {
    Q_OBJECT
public:
    OnexNStcData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;
    virtual ~OnexNStcData();

public slots:
    virtual int onReplace(QString directory) override;
    virtual void setWidth(int width);
    virtual void setHeight(int height);
};

#endif // ONEXNSTCDATA_H
