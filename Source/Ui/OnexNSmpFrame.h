#ifndef ONEXNSMPFRAME_H
#define ONEXNSMPFRAME_H
#include "OnexTreeImage.h"

class OnexNSmpFrame : public OnexTreeImage
{
private:
    int width;
    int height;
    int xOrigin;
    int yOrigin;
public:
    OnexNSmpFrame(QString name, QByteArray content, int width, int height, int xOrigin, int yOrigin, NosZlibOpener* opener, int id, int creationDate, bool compressed);

    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;

    int getWidth();
    int getHeight();
    int getXOrigin();
    int getYOrigin();
    QByteArray getContent();

    virtual ~OnexNSmpFrame();

private slots:
    virtual void onReplace() override;
};

#endif // ONEXNSMPFRAME_H
