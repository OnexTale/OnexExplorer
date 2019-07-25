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
    OnexNStgData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed);
    virtual ~OnexNStgData();
    virtual QWidget *onClicked() override;

private:
    NosModelConverter *modelConverter;

private slots:
    virtual int onExport(QString directory);
    virtual int onReplace(QString directory);
};

#endif // ONEXNSTGDATA_H
