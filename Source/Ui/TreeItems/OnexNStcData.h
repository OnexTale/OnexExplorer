#ifndef ONEXNSTCDATA_H
#define ONEXNSTCDATA_H

#include "OnexTreeImage.h"

class OnexNStcData : public OnexTreeImage {
Q_OBJECT
public:
    OnexNStcData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                 int creationDate, bool compressed);
    ~OnexNStcData() override;
    QImage getImage() override;
    ImageResolution getResolution() override;
public slots:
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false);
    void setHeight(int height, bool update = false);
protected:
    bool hasGoodResolution(int x, int y) override;
};

#endif // ONEXNSTCDATA_H
