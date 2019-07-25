#include "NosModelConverter.h"
#include <QDebug>
#include <QStringList>
#include <QtEndian>

NosModelConverter::NosModelConverter(QByteArray obj) {
    this->obj = obj;

    int offset = 0x36;
    float uvScale = fromLittleEndianToFloat(this->obj.mid(0x30, 4));
    int verticeCount = fromLittleEndianToShort(this->obj.mid(0x34, 2));
    this->vertices = readNosVertices(offset, verticeCount);
    this->uv = readNosUV(offset, verticeCount, uvScale);
    this->normals = readNosNormals(offset, verticeCount);
    this->faces = readNosFaces(offset);
    this->textures = readNosTextures(offset);
}

QVector<QVector3D> NosModelConverter::getVertices() {
    return vertices;
}
QVector<QVector3D> NosModelConverter::getNormals() {
    return normals;
}
QVector<QVector3D> NosModelConverter::getFaces() {
    return faces;
}

short NosModelConverter::fromLittleEndianToShort(QByteArray array) {
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

int NosModelConverter::fromLittleEndianToInt(QByteArray array) {
    return qFromLittleEndian<qint32>(reinterpret_cast<const uchar *>(array.data()));
}

float NosModelConverter::fromLittleEndianToFloat(QByteArray array) {
    return qFromLittleEndian<float>(reinterpret_cast<const uchar *>(array.data()));
}

QVector<QVector3D> NosModelConverter::readNosVertices(int &offset, int amount) {
    QVector<QVector3D> vertices;
    for (int i = 0; i < amount; i++) {
        float x = fromLittleEndianToFloat(this->obj.mid(offset, 4));
        float y = fromLittleEndianToFloat(this->obj.mid(offset + 4, 4));
        float z = fromLittleEndianToFloat(this->obj.mid(offset + 8, 4));
        vertices.append(QVector3D(x, y, z));
        offset += 12;
    }
    return vertices;
}
QVector<QVector3D> NosModelConverter::readNosNormals(int &offset, int amount) {
    QVector<QVector3D> normals;
    for (int i = 0; i < amount; i++) {
        float x = (float)this->obj.at(offset) / 0x7F;
        float y = (float)this->obj.at(offset + 1) / 0x7F;
        float z = (float)this->obj.at(offset + 2) / 0x7F;
        normals.append(QVector3D(x, y, z));
        offset += 3;
    }
    return normals;
}
QVector<QVector2D> NosModelConverter::readNosUV(int &offset, int amount, float scale) {
    QVector<QVector2D> uv;
    for (int i = 0; i < amount; i++) {
        float u = scale * fromLittleEndianToShort(this->obj.mid(offset, 2));
        float v = 1.0 - (scale * fromLittleEndianToShort(this->obj.mid(offset + 2, 2)));
        uv.append(QVector2D(u, v));
        offset += 4;
    }
    return uv;
}
QVector<QVector3D> NosModelConverter::readNosFaces(int &offset) {
    QVector<QVector3D> faces;
    int groups = fromLittleEndianToShort(this->obj.mid(offset, 2));
    offset += 2;

    for (int g = 0; g < groups; g++) {
        int points = (uint16_t)fromLittleEndianToShort(this->obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < points / 3; i++) {
            int p1 = fromLittleEndianToShort(this->obj.mid(offset, 2));
            int p2 = fromLittleEndianToShort(this->obj.mid(offset + 2, 2));
            int p3 = fromLittleEndianToShort(this->obj.mid(offset + 4, 2));
            faces.append(QVector3D(p1, p2, p3));
            offset += 6;
        }
    }
    return faces;
}

QString NosModelConverter::toObjFile(QString name) {
    QString obj = "";
    for (QVector3D v : vertices) {
        obj += "v " + QString::number(v.x()) + " " + QString::number(v.y()) + " " + QString::number(v.z()) + "\n";
    }
    for (QVector2D p : uv) {
        obj += "vt " + QString::number(p.x()) + " " + QString::number(p.y()) + "\n";
    }
    for (QVector3D vn : normals) {
        obj += "vn " + QString::number(vn.x()) + " " + QString::number(vn.y()) + " " + QString::number(vn.z()) + "\n";
    }
    obj += "\nmtllib " + name + ".mtl\n";
    for (int i = 0; i < groups.size(); i++) {
        obj += "\ng " + QString::number(i) + "\n";
        obj += "usemtl mtl-" + QString::number(textures.at(i)) + "\n";
        for (QVector3D f : readNosGroup(i)) {
            obj += "f " + QString::number(f.x() + 1) + "/" + QString::number(f.x() + 1) + "/" +
                   QString::number(f.x() + 1) + " " + QString::number(f.y() + 1) + "/" + QString::number(f.y() + 1) +
                   "/" + QString::number(f.y() + 1) + " " + QString::number(f.z() + 1) + "/" +
                   QString::number(f.z() + 1) + "/" + QString::number(f.z() + 1) + "\n";
        }
    }
    return obj;
}

QString NosModelConverter::toMtlFile() {
    QString mtl = "";
    for (int t : textures) {
        mtl += "newmtl mtl-" + QString::number(t) + "\n";
        mtl += "Ns 10\nKa 0 0 0\nKd 0 0 0\nKs 0 0 0\n";
        mtl += "map_Kd " + QString::number(t) + ".png\n";
    }
    return mtl;
}

QVector<int> NosModelConverter::readNosTextures(int &offset) {
    QVector<int> textures;
    int parts = fromLittleEndianToShort(this->obj.mid(offset, 2));
    offset += 2;

    for (int p = 0; p < parts; p++) {
        float x = fromLittleEndianToFloat(this->obj.mid(offset, 4));
        float y = fromLittleEndianToFloat(this->obj.mid(offset + 4, 4));
        float z = fromLittleEndianToFloat(this->obj.mid(offset + 8, 4));
        offset += 12;

        float rotation_x = (float)fromLittleEndianToShort(this->obj.mid(offset, 2)) / 0x7FFF;
        float rotation_y = (float)fromLittleEndianToShort(this->obj.mid(offset + 2, 2)) / 0x7FFF;
        float rotation_z = (float)fromLittleEndianToShort(this->obj.mid(offset + 4, 2)) / 0x7FFF;
        float rotation_w = (float)fromLittleEndianToShort(this->obj.mid(offset + 6, 2)) / 0x7FFF;
        offset += 8;

        float scale_x = fromLittleEndianToFloat(this->obj.mid(offset, 4));
        float scale_y = fromLittleEndianToFloat(this->obj.mid(offset + 4, 4));
        float scale_z = fromLittleEndianToFloat(this->obj.mid(offset + 8, 4));
        offset += 12;

        int translationFrameCount = fromLittleEndianToShort(this->obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < translationFrameCount; i++) {
            short time = fromLittleEndianToShort(this->obj.mid(offset, 2));
            float frame_x = fromLittleEndianToFloat(this->obj.mid(offset + 2, 4));
            float frame_y = fromLittleEndianToFloat(this->obj.mid(offset + 6, 4));
            float frame_z = fromLittleEndianToFloat(this->obj.mid(offset + 10, 4));
            offset += 14;
        }
        int rotationFrameCount = fromLittleEndianToShort(this->obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < rotationFrameCount; i++) {
            short time = fromLittleEndianToShort(this->obj.mid(offset, 2));
            float frame_rotation_x = (float)fromLittleEndianToShort(this->obj.mid(offset + 2, 2)) / 0x7FFF;
            float frame_rotation_y = (float)fromLittleEndianToShort(this->obj.mid(offset + 4, 2)) / 0x7FFF;
            float frame_rotation_z = (float)fromLittleEndianToShort(this->obj.mid(offset + 6, 2)) / 0x7FFF;
            float frame_rotation_w = (float)fromLittleEndianToShort(this->obj.mid(offset + 8, 2)) / 0x7FFF;
            offset += 10;
        }
        int scaleFrameCount = fromLittleEndianToShort(this->obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < scaleFrameCount; i++) {
            short time = fromLittleEndianToShort(this->obj.mid(offset, 2));
            float frame_scale_x = fromLittleEndianToFloat(this->obj.mid(offset + 2, 4));
            float frame_scale_y = fromLittleEndianToFloat(this->obj.mid(offset + 6, 4));
            float frame_scale_z = fromLittleEndianToFloat(this->obj.mid(offset + 10, 4));
            offset += 14;
        }

        int c = fromLittleEndianToShort(this->obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < c; i++) {
            int img = fromLittleEndianToInt(this->obj.mid(offset, 4));
            offset += 4;
            bool smooth = this->obj.at(offset); // maybe means something else
            offset++;
            int grp = fromLittleEndianToShort(this->obj.mid(offset, 2));
            textures.append(img); // TODO!!!
            offset += 2;
        }
        fromLittleEndianToShort(this->obj.mid(offset, 2)); // seems to be always 0?
        offset += 2;
    }

    return textures;
}

QVector<QVector3D> NosModelConverter::readNosGroup(int group) {
    QVector<QVector3D> faceGroup;

    int end = 0;
    if (group + 1 < groups.size())
        end = groups[group + 1];
    else
        end = groups[groups.size() - 1];

    for (int i = groups[group]; i < end; i++) {
        faceGroup.append(faces[i]);
    }

    return faceGroup;
}
