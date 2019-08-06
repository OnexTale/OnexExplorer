#ifndef OBJCONVERTER_H
#define OBJCONVERTER_H
#include "IModelConverter.h"
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class ObjConverter : public IModelConverter {
private:
    QString generateObjFile(Model model, QString name);
    QString generateMtlFile(Model model);

public:
    ObjConverter();
    Model fromObj(QString obj);
    QStringList toObj(Model model, QString name);
};

#endif // OBJCONVERTER_H
