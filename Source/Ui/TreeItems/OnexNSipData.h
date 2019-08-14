#ifndef ONEXNSIPDATA_H
#define ONEXNSIPDATA_H

#include "OnexTreeImage.h"

class OnexNSipData : public OnexTreeImage {
Q_OBJECT
public:
    OnexNSipData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                 bool compressed);
    ~OnexNSipData() override;
    QImage getImage() override;
    ImageResolution getResolution() override;
    ImageResolution getCenter();
public slots:
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false);
    void setHeight(int height, bool update = false);
    void setCenterX(int center, bool update = false);
    void setCenterY(int center, bool update = false);
protected:
    FileInfo *generateInfos() override;
};

#endif // ONEXNSIPDATA_H
