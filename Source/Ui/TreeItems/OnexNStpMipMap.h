#ifndef ONEXNSTPMAPMAP_H
#define ONEXNSTPMAPMAP_H

#include "OnexTreeImage.h"

class OnexNStpMipMap : public OnexTreeImage {
Q_OBJECT
public:
    OnexNStpMipMap(QString name, QByteArray content, int width, int height, int format, NosZlibOpener *opener,
                   int id, int creationDate, bool compressed);

    ~OnexNStpMipMap() override;
    int onReplace(QString directory) override;
    QImage getImage() override;
    ImageResolution getResolution() override;
    int getWidth();
    int getHeight();
    int getFormat();
public slots:
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false) override;
    void setHeight(int height, bool update = false) override;
    void setFormat(uint8_t format, bool update = false);
protected:
    int width;
    int height;
    int format;
    FileInfo *generateInfos() override;
};

#endif // ONEXNSTPMAPMAP_H
