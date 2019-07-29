#ifndef NOSMODELCONVERTER_H
#define NOSMODELCONVERTER_H
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

class NosModelConverter {
private:
    QByteArray obj;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> uv;
    QVector<QVector3D> faces;
    QVector<int> groups;
    QVector<int> textures;
    QVector<QVector3D> positions;
    QVector<QVector4D> rotations;
    QVector<QVector3D> scales;
    int objectInfoOffset;
    short fromLittleEndianToShort(QByteArray array);
    int fromLittleEndianToInt(QByteArray array);
    float fromLittleEndianToFloat(QByteArray array);
    QVector<QVector3D> readNosVertices(int &offset, int amount);
    QVector<QVector3D> readNosNormals(int &offset, int amount);
    QVector<QVector2D> readNosUV(int &offset, int amount, float scale);
    QVector<QVector3D> readNosFaces(int &offset);
    QVector<int> readNosTextures(int &offset);
    QVector<QVector3D> readNosGroup(int group);

public:
    NosModelConverter(QByteArray obj);
    QString toObjFile(QString name);
    QString toMtlFile();
    QVector<QVector3D> getVertices();
    QVector<QVector3D> getNormals();
    QVector<QVector3D> getFaces();
    QVector3D getPosition(int index);
    QVector4D getRotation(int index);
    QVector3D getScale(int index);
    int getObjectInfoOffset();
};

#endif // NOSMODELCONVERTER_H