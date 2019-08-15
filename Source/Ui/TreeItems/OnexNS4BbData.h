#ifndef ONEXNS4BBDATA_H
#define ONEXNS4BBDATA_H

#include "OnexTreeImage.h"

class OnexNS4BbData : public OnexTreeImage {
Q_OBJECT
public:
    OnexNS4BbData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                  int creationDate, bool compressed);
    ~OnexNS4BbData() override;
    QImage getImage() override;
    ImageResolution getResolution() override;
public slots:
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false) override;
    void setHeight(int height, bool update = false) override;
};

#endif // ONEXNS4BBDATA_H
