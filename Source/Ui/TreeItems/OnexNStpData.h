#ifndef ONEXNSTPDATA_H
#define ONEXNSTPDATA_H

#include "OnexTreeImage.h"

class OnexNStpData : public OnexTreeImage {
Q_OBJECT
public:
    OnexNStpData(const QByteArray &header, const QString &name, const QByteArray &content, NosZlibOpener *opener,
                 int id, int creationDate, bool compressed);

    ~OnexNStpData() override;
    QByteArray getContent() override;
    QImage getImage() override;
    ImageResolution getResolution() override;
    int getFormat();
    int getFileAmount();
public slots:
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false) override;
    void setHeight(int height, bool update = false) override;
    void setFormat(uint8_t format, bool update = false);
protected:
    FileInfo *generateInfos() override;
    void generateMipMap();
};

#endif // ONEXNSTPDATA_H
