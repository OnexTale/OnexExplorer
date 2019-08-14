#ifndef OBJCONVERTER_H
#define OBJCONVERTER_H

#include "IModelConverter.h"
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

struct Face {
    int object;
    int group;
    QVector3D v;
    QVector3D vt;
    QVector3D vn;
};

class ObjConverter : public IModelConverter {
public:
    ObjConverter();
    Model *fromObj(const QString &obj);
    QStringList toObj(Model *model, QString name);
private:
    QString generateObjFile(Model *model, const QString &name);
    QString generateMtlFile(Model *model);
};

#endif // OBJCONVERTER_H
