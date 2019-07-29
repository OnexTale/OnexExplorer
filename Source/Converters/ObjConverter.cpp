#include "ObjConverter.h"
#include <QDebug>
#include <QStringList>
#include <QtEndian>

ObjConverter::ObjConverter(QString obj) {
    this->obj = obj;
}

QByteArray ObjConverter::fromShortToLittleEndian(short number) {
    QByteArray writeArray;
    writeArray.resize(2);
    qToLittleEndian<qint16>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray ObjConverter::fromIntToLittleEndian(int number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<qint32>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray ObjConverter::fromFloatToLittleEndian(float number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<float>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QByteArray ObjConverter::getNosFormat(float uvScale) {
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> uv;
    QVector<QVector3D> facesVertice;
    QVector<QVector3D> facesUV;
    QVector<QVector3D> facesNormal;
    QVector<int> groups;
    QVector<int> textures;
    QVector<QVector3D> normalPerVertex;
    QVector<QVector2D> uvPerVertex;

    QStringList lines = obj.split("\n");
    for (QString line : lines) {
        QStringList parts = line.split(" ", QString::SplitBehavior::SkipEmptyParts);
        if (parts.size() > 0) {
            if (parts[0] == "v") {
                vertices.append(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            } else if (parts[0] == "vn") {
                normals.append(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            } else if (parts[0] == "vt") {
                uv.append(QVector2D(parts[1].toFloat(), parts[2].toFloat()));
            } else if (parts[0] == "g") {
                //    groups.append(facesVertice.size());
            } else if (parts[0] == "f") {
                QStringList faceOneInfos = parts[1].split("/");
                QStringList faceTwoInfos = parts[2].split("/");
                QStringList faceThreeInfos = parts[3].split("/");

                facesVertice.append(QVector3D(faceOneInfos[0].toFloat() - 1, faceTwoInfos[0].toFloat() - 1,
                                              faceThreeInfos[0].toFloat() - 1));
                facesUV.append(QVector3D(faceOneInfos[1].toFloat() - 1, faceTwoInfos[1].toFloat() - 1,
                                         faceThreeInfos[1].toFloat() - 1));
                facesNormal.append(QVector3D(faceOneInfos[2].toFloat() - 1, faceTwoInfos[2].toFloat() - 1,
                                             faceThreeInfos[2].toFloat() - 1));
            } else if (parts[0] == "usemtl") {
                QStringList texturemtl = parts[1].split("mtl-", QString::SplitBehavior::SkipEmptyParts);
                groups.append(facesVertice.size());
                textures.append(texturemtl.at(0).toInt());
            }
        }
    }

    normalPerVertex.resize(vertices.size());
    uvPerVertex.resize(vertices.size());
    for (int i = 0; i < facesVertice.size(); i++) {
        normalPerVertex[facesVertice[i].x()] = normals[facesNormal[i].x()];
        normalPerVertex[facesVertice[i].y()] = normals[facesNormal[i].y()];
        normalPerVertex[facesVertice[i].z()] = normals[facesNormal[i].z()];

        uvPerVertex[facesVertice[i].x()] = uv[facesUV[i].x()];
        uvPerVertex[facesVertice[i].y()] = uv[facesUV[i].y()];
        uvPerVertex[facesVertice[i].z()] = uv[facesUV[i].z()];
    }

    QByteArray newContent;
    newContent.append(fromFloatToLittleEndian(uvScale));
    newContent.append(fromShortToLittleEndian(vertices.size()));
    for (int i = 0; i < vertices.size(); i++) {
        newContent.append(fromFloatToLittleEndian(vertices[i].x()));
        newContent.append(fromFloatToLittleEndian(vertices[i].y()));
        newContent.append(fromFloatToLittleEndian(vertices[i].z()));
    }
    for (int i = 0; i < uvPerVertex.size(); i++) {
        float u = uvPerVertex[i].x();
        if ((int)u != 1 && (int)u != -1)
            u -= (int)u;
        u /= uvScale;
        float v = uvPerVertex[i].y();
        if ((int)v != 1 && (int)v != -1)
            v -= (int)v;
        v = (1.0 - v) / uvScale;
        newContent.append(fromShortToLittleEndian(u));
        newContent.append(fromShortToLittleEndian(v));
    }
    for (int i = 0; i < normalPerVertex.size(); i++) {
        uint8_t x = normalPerVertex[i].x() * 0x7F;
        uint8_t y = normalPerVertex[i].y() * 0x7F;
        uint8_t z = normalPerVertex[i].z() * 0x7F;
        newContent.append(x);
        newContent.append(y);
        newContent.append(z);
    }
    newContent.append(fromShortToLittleEndian(groups.size()));
    for (int g = 0; g < groups.size(); g++) {
        int grpEnd = 0;
        if (g + 1 < groups.size())
            grpEnd = groups[g + 1];
        else
            grpEnd = facesVertice.size();
        newContent.append(fromShortToLittleEndian((grpEnd - groups[g]) * 3)); // GroupSize
        for (int i = groups[g]; i < grpEnd; i++) {
            newContent.append(fromShortToLittleEndian(facesVertice[i].x()));
            newContent.append(fromShortToLittleEndian(facesVertice[i].y()));
            newContent.append(fromShortToLittleEndian(facesVertice[i].z()));
        }
    }
    newContent.append(fromShortToLittleEndian(1)); // ObjectCount

    newContent.append(fromFloatToLittleEndian(0.34155));   // x
    newContent.append(fromFloatToLittleEndian(-2.403));    // y
    newContent.append(fromFloatToLittleEndian(-0.318199)); // z

    newContent.append(fromShortToLittleEndian(0)); // x rotation
    newContent.append(fromShortToLittleEndian(0)); // y rotation
    newContent.append(fromShortToLittleEndian(0)); // z rotation
    newContent.append(fromShortToLittleEndian(0)); // w rotation

    newContent.append(fromFloatToLittleEndian(1)); // x scale
    newContent.append(fromFloatToLittleEndian(1)); // y scale
    newContent.append(fromFloatToLittleEndian(1)); // z scale

    newContent.append(fromShortToLittleEndian(0)); // translationFrameCount
    newContent.append(fromShortToLittleEndian(0)); // rotationFrameCount
    newContent.append(fromShortToLittleEndian(0)); // scaleFrameCount

    newContent.append(fromShortToLittleEndian(groups.size()));
    for (int i = 0; i < groups.size(); i++) {
        newContent.append(fromIntToLittleEndian(textures[i]));
        newContent.append((char)0);                    // bool
        newContent.append(fromShortToLittleEndian(i)); // group
    }
    newContent.append(fromShortToLittleEndian(0));

    return newContent;
}