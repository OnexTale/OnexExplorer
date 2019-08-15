#ifndef NOSMODELCONVERTER_H
#define NOSMODELCONVERTER_H

#include "IModelConverter.h"
#include "LittleEndianConverter.h"
#include <QByteArray>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QVector>

class NosModelConverter : public IModelConverter {
public:
    NosModelConverter();
    Model *fromBinary(const QByteArray &obj);
    QByteArray toBinary(Model *model);
private:
    QVector<QVector3D> readNosVertices(const QByteArray &obj, int &offset, int verticesCount);
    QVector<QVector3D> readNosNormals(const QByteArray &obj, int &offset, int verticesCount);
    QVector<QVector2D> readNosUV(const QByteArray &obj, int &offset, int verticesCount, float uvScale);
    QVector<ModelGroup> readNosFaces(const QByteArray &obj, int &offset);
    Model *readModelConstruction(const QByteArray &obj, int &offset, Model *model, QVector<ModelGroup> groups);
};

#endif // NOSMODELCONVERTER_H
