#ifndef ONEXNSTPDATA_H
#define ONEXNSTPDATA_H
#include "OnexTreeImage.h"

class OnexNStpData : public OnexTreeImage {
    Q_OBJECT
public:
    OnexNStpData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    virtual QByteArray getContent() override;
    int getFormat();
    int getFileAmount();
    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;
    virtual QWidget *getInfos() override;
    virtual ~OnexNStpData();

public slots:
    virtual int onReplace(QString directory) override;
    virtual int onExport(QString directory) override;
    virtual void setWidth(int width);
    virtual void setHeight(int height);
    virtual void setFormat(uint8_t format);
    virtual void setFormat(QString format);
};

#endif // ONEXNSTPDATA_H
