#ifndef ONEXNSMPFRAME_H
#define ONEXNSMPFRAME_H
#include "OnexTreeImage.h"

class OnexNSmpFrame : public OnexTreeImage {
    Q_OBJECT
private:
    int width;
    int height;
    int xOrigin;
    int yOrigin;

public:
    OnexNSmpFrame(QByteArray header, QString name, QByteArray content, int width, int height, int xOrigin, int yOrigin,
                  NosZlibOpener *opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;
    virtual QWidget *getInfos() override;
    int getWidth();
    int getHeight();
    int getXOrigin();
    int getYOrigin();
    QByteArray getContent();
    virtual ~OnexNSmpFrame();

public slots:
    virtual int onReplace(QString directory) override;
    virtual void setWidth(int width);
    virtual void setHeight(int height);
    virtual void setXOrigin(int xOrigin);
    virtual void setYOrigin(int yOrigin);
    virtual void setXOrigin(QString xOrigin);
    virtual void setYOrigin(QString yOrigin);
};

#endif // ONEXNSMPFRAME_H
