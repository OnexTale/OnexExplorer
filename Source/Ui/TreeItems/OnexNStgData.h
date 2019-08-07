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
    virtual void setXPosition(int index, float x);
    virtual void setYPosition(int index, float y);
    virtual void setZPosition(int index, float z);
    virtual void setXRotation(int index, float x);
    virtual void setYRotation(int index, float y);
    virtual void setZRotation(int index, float z);
    virtual void setWRotation(int index, float w);
    virtual void setXScale(int index, float x);
    virtual void setYScale(int index, float y);
    virtual void setZScale(int index, float z);
    virtual void setTexture(int index, int texture);

signals:
    void replaceSignal(Model *newModel);
};

#endif // ONEXNSTGDATA_H
