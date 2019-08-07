#ifndef ONEXTREEIMAGE_H
#define ONEXTREEIMAGE_H
#include "OnexTreeZlibItem.h"
#include "../Previews/SingleImagePreview.h"
#include "../../Converters/ImageConverter.h"

struct ImageResolution {
    int x;
    int y;
};

class OnexTreeImage : public OnexTreeZlibItem {
    Q_OBJECT
protected:
    bool hasGoodResolution(int x, int y);
    QImage importQImageFromSelectedUserFile(QString filepath);

public:
    OnexTreeImage(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    static ImageConverter imageConverter;
    virtual QWidget *getPreview() override;
    virtual FileInfo *getInfos() override;
    virtual QImage getImage() = 0;
    virtual ImageResolution getResolution() = 0;

public slots:
    virtual int onExport(QString directory);
    virtual void setWidth(int width) = 0;
    virtual void setHeight(int height) = 0;
    virtual QString getExportExtension();

signals:
    void replaceSignal(QImage newImage);
};

#endif // ONEXTREEIMAGE_H
