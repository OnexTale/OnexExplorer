#ifndef ONEXNSTPDATA_H
#define ONEXNSTPDATA_H
#include "OnexTreeImage.h"

class OnexNStpData : public OnexTreeImage {
    Q_OBJECT

private:
    FileInfo *generateInfos() override;
public:
    OnexNStpData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    virtual QByteArray getContent() override;
    int getFormat();
    int getFileAmount();
    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;
    virtual FileInfo *getInfos() override;
    virtual ~OnexNStpData();

public slots:
    virtual int onReplace(QString directory) override;
    virtual int onExport(QString directory) override;
    virtual void setWidth(int width, bool update = false);
    virtual void setHeight(int height, bool update = false);
    virtual void setFormat(uint8_t format, bool update = false);
};

#endif // ONEXNSTPDATA_H
