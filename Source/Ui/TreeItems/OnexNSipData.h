#ifndef ONEXNSIPDATA_H
#define ONEXNSIPDATA_H
#include "OnexTreeImage.h"

class OnexNSipData : public OnexTreeImage {
    Q_OBJECT
private:
    FileInfo *generateInfos() override;
public:
    OnexNSipData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    FileInfo *getInfos() override;
    virtual ImageResolution getResolution() override;
    virtual ImageResolution getCenter();
    virtual ~OnexNSipData();

public slots:
    virtual int onReplace(QString directory) override;
    virtual void setWidth(int width, bool update = false);
    virtual void setHeight(int height, bool update = false);
    virtual void setCenterX(int center, bool update = false);
    virtual void setCenterY(int center, bool update = false);
};

#endif // ONEXNSIPDATA_H
