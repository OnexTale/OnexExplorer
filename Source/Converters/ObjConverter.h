#ifndef OBJCONVERTER_H
#define OBJCONVERTER_H
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class ObjConverter {
private:
    QString obj;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> uv;
    QVector<int> textures;
    QByteArray fromShortToLittleEndian(short number);
    QByteArray fromIntToLittleEndian(int number);
    QByteArray fromFloatToLittleEndian(float number);

public:
    ObjConverter(QString obj);
    QByteArray getNosFormat(float uvScale);
};

#endif // OBJCONVERTER_H
