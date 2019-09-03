#ifndef ONEXNSTPMAPMAP_H
#define ONEXNSTPMAPMAP_H

#include "OnexTreeImage.h"

class OnexNStpMipMap : public OnexTreeImage {
Q_OBJECT
public:
    OnexNStpMipMap(QString name, QByteArray content, int width, int height, int format, NosZlibOpener *opener,
                   int id = -1, int creationDate = 0, bool compressed = false);
    OnexNStpMipMap(QJsonObject jo, NosZlibOpener *opener, const QString &directory);
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
