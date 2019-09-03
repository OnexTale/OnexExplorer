#ifndef ONEXNSTPDATA_H
#define ONEXNSTPDATA_H

#include "OnexTreeImage.h"

class OnexNStpData : public OnexTreeImage {
Q_OBJECT
public:
    OnexNStpData(const QString &name, QByteArray content, NosZlibOpener *opener,
                 int id = -1, int creationDate = 0, bool compressed = false);
    OnexNStpData(QJsonObject jo, NosZlibOpener *opener, const QString &directory);
    ~OnexNStpData() override;
    QByteArray getContent() override;
    QImage getImage() override;
    ImageResolution getResolution() override;
    int getFormat();
    bool getSmoothScaling();
    bool getUnknownValue();
    int getFileAmount();
public slots:
    void setName(QString name);
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false) override;
    void setHeight(int height, bool update = false) override;
    void setFormat(uint8_t format, bool update = false);
    void setSmoothScaling(bool smooth, bool update = false);
    void setUnknownValue(bool unkown, bool update = false);
    void setFileAmount(uint8_t format, bool update = false);
protected:
    FileInfo *generateInfos() override;
    void generateMipMap(bool generate);
};

#endif // ONEXNSTPDATA_H
