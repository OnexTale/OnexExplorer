#ifndef ONEXNSTGDATA_H
#define ONEXNSTGDATA_H
#include "../Openers/NosZlibOpener.h"
#include "OnexTreeZlibItem.h"
#include "../NosModelConverter.h"
#include "../ObjConverter.h"
#include <QVector2D>
#include <QVector3D>

class OnexNStgData : public OnexTreeZlibItem {
Q_OBJECT
public:
    OnexNStgData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    virtual ~OnexNStgData();
    virtual QWidget *getPreview() override;
    virtual QWidget *getInfos() override;

private:
    NosModelConverter *modelConverter;

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
    virtual void setXPosition(QString x);
    virtual void setYPosition(QString y);
    virtual void setZPosition(QString z);
    virtual void setXRotation(QString x);
    virtual void setYRotation(QString y);
    virtual void setZRotation(QString z);
    virtual void setWRotation(QString w);
    virtual void setXScale(QString x);
    virtual void setYScale(QString y);
    virtual void setZScale(QString z);
};

#endif // ONEXNSTGDATA_H
