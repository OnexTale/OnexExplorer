#ifndef NOSMODELCONVERTER_H
#define NOSMODELCONVERTER_H
#include "IModelConverter.h"
#include <QByteArray>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QVector>

class NosModelConverter : public IModelConverter {
private:
    QVector<QVector3D> readNosVertices(QByteArray obj, int &offset, int verticeCount);
    QVector<QVector3D> readNosNormals(QByteArray obj, int &offset, int verticeCount);
    QVector<QVector2D> readNosUV(QByteArray obj, int &offset, int verticeCount, float uvScale);
    QVector<ModelGroup> readNosFaces(QByteArray obj, int &offset);
    Model *readModelConstruction(QByteArray obj, int &offset, Model *model, QVector<ModelGroup> groups);

public:
    NosModelConverter();
    Model *fromBinary(QByteArray obj);
    QByteArray toBinary(Model *model);
};

#endif // NOSMODELCONVERTER_H
