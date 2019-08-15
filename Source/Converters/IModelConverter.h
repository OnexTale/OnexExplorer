#ifndef IMODELCONVERTER_H
#define IMODELCONVERTER_H

#include <QByteArray>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include "LittleEndianConverter.h"

struct ModelAnimationPositionFrame {
    int timestamp;
    QVector3D position;
};
struct ModelAnimationRotationFrame {
    int timestamp;
    QVector4D rotation;
};
struct ModelAnimationScaleFrame {
    int timestamp;
    QVector3D scale;
};
struct ModelGroup {
    int number;
    QVector<QVector3D> faces;
    int texture;
};
struct ModelObject {
    QVector<int> groups;
    QVector3D position;
    QVector4D rotation;
    QVector3D scale;
    QVector<ModelAnimationPositionFrame> animationPositions;
    QVector<ModelAnimationRotationFrame> animationRotations;
    QVector<ModelAnimationScaleFrame> animationScales;
};
struct Model {
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> uv;
    QVector<ModelGroup> groups;
    QVector<ModelObject> objects;
    float uvScale;
};

class IModelConverter {
protected:
    LittleEndianConverter *littleEndianConverter;
};

#endif // IMODELCONVERTER_H
