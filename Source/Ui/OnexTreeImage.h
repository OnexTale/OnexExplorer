#ifndef ONEXTREEIMAGE_H
#define ONEXTREEIMAGE_H
#include "OnexTreeZlibItem.h"
#include "SingleImagePreview.h"

struct ImageResolution
{
    int x;
    int y;
};

class OnexTreeImage : public OnexTreeZlibItem
{
private:

public:
    OnexTreeImage(QString name, QByteArray content, NosZlibOpener* opener, int id, int creationDate, bool compressed);

    virtual QWidget *onClicked() override;
    virtual QImage getImage() = 0;
    virtual ImageResolution getResolution() = 0;

private slots:
    virtual void onExportAll();

};

#endif // ONEXTREEIMAGE_H
