#ifndef ONEXNS4BBDATA_H
#define ONEXNS4BBDATA_H

#include "OnexTreeImage.h"

class OnexNS4BbData : public OnexTreeImage {
Q_OBJECT
public:
    OnexNS4BbData(QString name, QByteArray content, NosZlibOpener *opener, int id = -1,
                  int creationDate = 0, bool compressed = false);
    OnexNS4BbData(QJsonObject jo, NosZlibOpener *opener, const QString &directory);
    ~OnexNS4BbData() override;
    QImage getImage() override;
    ImageResolution getResolution() override;
public slots:
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false) override;
    void setHeight(int height, bool update = false) override;
};

#endif // ONEXNS4BBDATA_H
