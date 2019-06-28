#ifndef ONEXNSTPMAPMAP_H
#define ONEXNSTPMAPMAP_H
#include "OnexNStpData.h"

class OnexNStpMipMap : public OnexNStpData {
private:
    int width;
    int height;
    int format;

public:
    OnexNStpMipMap(QString name, QByteArray content, int width, int height, int format,
                   NosZlibOpener *opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;

    int getWidth();
    int getHeight();
    int getFormat();
    QByteArray getContent();
    virtual ~OnexNStpMipMap();

public slots:
    virtual int onReplace(QString directory) override;
};

#endif // ONEXNSTPMAPMAP_H
