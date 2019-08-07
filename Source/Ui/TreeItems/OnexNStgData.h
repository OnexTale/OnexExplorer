#ifndef ONEXNSTGDATA_H
#define ONEXNSTGDATA_H
#include "../../Openers/NosZlibOpener.h"
#include "OnexTreeZlibItem.h"
#include "../../Converters/NosModelConverter.h"
#include "../../Converters/ObjConverter.h"
#include <QVector2D>
#include <QVector3D>

class OnexNStgData : public OnexTreeZlibItem {
Q_OBJECT
public:
    OnexNStgData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    virtual ~OnexNStgData();
    virtual QWidget *getPreview() override;
    virtual FileInfo *getInfos() override;
    virtual QByteArray getContent() override;

public slots:
    virtual QString getExportExtension();

private:
    Model *model;

private slots:
    virtual int onExport(QString directory);
    virtual int onReplace(QString directory);
    virtual void setXPosition(float x);
    virtual void setYPosition(float y);
    virtual void setZPosition(float z);
    virtual void setXRotation(float x);
    virtual void setYRotation(float y);
    virtual void setZRotation(float z);
    virtual void setWRotation(float w);
    virtual void setXScale(float x);
    virtual void setYScale(float y);
    virtual void setZScale(float z);
    virtual void setTexture(int index, int texture);

signals:
    void replaceSignal(Model *newModel);
};

#endif // ONEXNSTGDATA_H
