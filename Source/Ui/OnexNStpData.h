#ifndef ONEXNSTPDATA_H
#define ONEXNSTPDATA_H
#include "OnexTreeImage.h"

class OnexNStpData : public OnexTreeImage {
public:
    OnexNStpData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);

    int getFormat();
    int getFileAmount();
    virtual QImage getImage() override;
    virtual ImageResolution getResolution() override;

    virtual ~OnexNStpData();

private slots:
    virtual int onReplace(QString directory) override;
    virtual int onExportAll(QString directory) override;
    virtual int onExportSingle(QString directory) override;
};

#endif // ONEXNSTPDATA_H
