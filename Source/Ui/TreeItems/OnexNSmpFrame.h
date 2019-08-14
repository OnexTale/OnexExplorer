#ifndef ONEXNSMPFRAME_H
#define ONEXNSMPFRAME_H

#include "OnexTreeImage.h"

class OnexNSmpFrame : public OnexTreeImage {
Q_OBJECT
public:
    OnexNSmpFrame(QByteArray header, QString name, QByteArray content, int width, int height, int xOrigin, int yOrigin,
                  NosZlibOpener *opener, int id, int creationDate, bool compressed);
    ~OnexNSmpFrame() override;
    FileInfo *getInfos() override;
    QImage getImage() override;
    ImageResolution getResolution() override;
    int getWidth();
    int getHeight();
    int getXOrigin();
    int getYOrigin();
public slots:
    int afterReplace(QImage image) override;
    void setWidth(int width, bool update = false) override;
    void setHeight(int height, bool update = false) override;
    void setXOrigin(int xOrigin, bool update = false);
    void setYOrigin(int yOrigin, bool update = false);
protected:
    int width;
    int height;
    int xOrigin;
    int yOrigin;
    FileInfo *generateInfos() override;
};

#endif // ONEXNSMPFRAME_H
