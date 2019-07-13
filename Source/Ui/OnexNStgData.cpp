#include "OnexNStgData.h"
#include "SingleModelPreview.h"

OnexNStgData::OnexNStgData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeZlibItem(name, content, opener, id, creationDate, compressed) {
}

QWidget *OnexNStgData::onClicked() {
    if (!hasParent())
        return nullptr;
    getTextures();
    return new SingleModelPreview(getVertices(), getFaces(), getNormals());
}

int OnexNStgData::getVerticeCount() {
    return fromLittleEndianToShort(content.mid(0x34, 2));
}

float OnexNStgData::getUVScale() {
    return fromLittleEndianToFloat(content.mid(0x30, 4));
}

QVector<QVector3D> OnexNStgData::getVertices() {
    QVector<QVector3D> vertices;

    int amount = getVerticeCount();
    int offset = 0x36;
    for (int i = 0; i < amount; i++) {
        float x = fromLittleEndianToFloat(content.mid(offset, 4));
        float y = fromLittleEndianToFloat(content.mid(offset + 4, 4));
        float z = fromLittleEndianToFloat(content.mid(offset + 8, 4));
        vertices.append(QVector3D(x, y, z));
        offset += 12;
    }
    return vertices;
}

QVector<QVector3D> OnexNStgData::getFaces() {
    QVector<QVector3D> faces;
    int offset = 0x36 + 19 * getVerticeCount();
    int groups = fromLittleEndianToShort(content.mid(offset, 2));
    offset += 2;

    for (int g = 0; g < groups; g++) {
        int points = fromLittleEndianToShort(content.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < points / 3; i++) {
            int p1 = fromLittleEndianToShort(content.mid(offset, 2));
            int p2 = fromLittleEndianToShort(content.mid(offset + 2, 2));
            int p3 = fromLittleEndianToShort(content.mid(offset + 4, 2));
            faces.append(QVector3D(p1, p2, p3));
            offset += 6;
        }
    }
    return faces;
}

int OnexNStgData::getGroupCount() {
    return fromLittleEndianToShort(content.mid(0x36 + 19 * getVerticeCount(), 2));
}

QVector<QVector3D> OnexNStgData::getGroup(int group) {
    QVector<QVector3D> faces;
    int offset = 0x36 + 19 * getVerticeCount() + 2;

    for (int i = 0; i < group; i++) {
        int c = fromLittleEndianToShort(content.mid(offset, 2));
        offset += 2 * c + 2;
    }

    int points = fromLittleEndianToShort(content.mid(offset, 2));
    offset += 2;
    for (int i = 0; i < points / 3; i++) {
        int p1 = fromLittleEndianToShort(content.mid(offset, 2));
        int p2 = fromLittleEndianToShort(content.mid(offset + 2, 2));
        int p3 = fromLittleEndianToShort(content.mid(offset + 4, 2));
        faces.append(QVector3D(p1, p2, p3));
        offset += 6;
    }

    return faces;
}

QVector<QVector3D> OnexNStgData::getNormals() {
    QVector<QVector3D> normals;
    int offset = 0x36 + 16 * getVerticeCount();
    for (int i = 0; i < getVerticeCount(); i++) {
        float x = (float)content.at(offset) / 0x7F;
        float y = (float)content.at(offset + 1) / 0x7F;
        float z = (float)content.at(offset + 2) / 0x7F;
        normals.append(QVector3D(x, y, z));
        offset += 3;
    }
    return normals;
}

QVector<QVector2D> OnexNStgData::getUVMap() {
    QVector<QVector2D> uv;
    int offset = 0x36 + 12 * getVerticeCount();
    for (int i = 0; i < getVerticeCount(); i++) {
        float u = getUVScale() * fromLittleEndianToShort(content.mid(offset, 2));
        float v = 1.0 - (getUVScale() * fromLittleEndianToShort(content.mid(offset + 2, 2)));
        offset += 4;
        uv.append(QVector2D(u, v));
    }
    return uv;
}

QVector<int> OnexNStgData::getTextures() {
    int offset = 0x36 + 19 * getVerticeCount() + 2;
    for (int i = 0; i < getGroupCount(); i++) {
        int c = fromLittleEndianToShort(content.mid(offset, 2));
        offset += 2 * c + 2;
    }

    QVector<int> textures;
    int parts = fromLittleEndianToShort(content.mid(offset, 2));
    offset += 2;

    for (int p = 0; p < parts; p++) {
        float x = fromLittleEndianToFloat(content.mid(offset, 4));
        float y = fromLittleEndianToFloat(content.mid(offset + 4, 4));
        float z = fromLittleEndianToFloat(content.mid(offset + 8, 4));
        offset += 12;

        float rotation_x = fromLittleEndianToFloat(content.mid(offset, 2)) / 0x7FFF;
        float rotation_y = fromLittleEndianToFloat(content.mid(offset + 2, 2)) / 0x7FFF;
        float rotation_z = fromLittleEndianToFloat(content.mid(offset + 4, 2)) / 0x7FFF;
        float rotation_w = fromLittleEndianToFloat(content.mid(offset + 6, 2)) / 0x7FFF;
        offset += 8;

        float scale_x = fromLittleEndianToFloat(content.mid(offset, 4));
        float scale_y = fromLittleEndianToFloat(content.mid(offset + 4, 4));
        float scale_z = fromLittleEndianToFloat(content.mid(offset + 8, 4));
        offset += 12;

        int translationFrameCount = fromLittleEndianToShort(content.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < translationFrameCount; i++) {
            short time = fromLittleEndianToShort(content.mid(offset, 2));
            float frame_x = fromLittleEndianToFloat(content.mid(offset + 2, 4));
            float frame_y = fromLittleEndianToFloat(content.mid(offset + 6, 4));
            float frame_z = fromLittleEndianToFloat(content.mid(offset + 10, 4));
            offset += 14;
        }
        int rotationFrameCount = fromLittleEndianToShort(content.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < rotationFrameCount; i++) {
            short time = fromLittleEndianToShort(content.mid(offset, 2));
            float frame_rotation_x = (float)fromLittleEndianToShort(content.mid(offset + 2, 2)) / 0x7FFF;
            float frame_rotation_y = (float)fromLittleEndianToShort(content.mid(offset + 4, 2)) / 0x7FFF;
            float frame_rotation_z = (float)fromLittleEndianToShort(content.mid(offset + 6, 2)) / 0x7FFF;
            float frame_rotation_w = (float)fromLittleEndianToShort(content.mid(offset + 8, 2)) / 0x7FFF;
            offset += 10;
        }
        int scaleFrameCount = fromLittleEndianToShort(content.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < scaleFrameCount; i++) {
            short time = fromLittleEndianToShort(content.mid(offset, 2));
            float frame_scale_x = fromLittleEndianToFloat(content.mid(offset + 2, 4));
            float frame_scale_y = fromLittleEndianToFloat(content.mid(offset + 6, 4));
            float frame_scale_z = fromLittleEndianToFloat(content.mid(offset + 10, 4));
            offset += 14;
        }

        int c = fromLittleEndianToShort(content.mid(offset, 2));
        offset += 2;

        for (int i = 0; i < c; i++) {
            int img = fromLittleEndianToInt(content.mid(offset, 4));
            offset += 4;
            bool smooth = content.at(offset); // maybe means something else
            offset++;
            int grp = fromLittleEndianToShort(content.mid(offset, 2));
            textures.append(img); // TODO!!!
            offset += 2;
        }
        qDebug() << fromLittleEndianToShort(content.mid(offset,2)); //seems to be always 0?
        offset += 2;
    }

    return textures;
}

int OnexNStgData::onExport(QString directory) {
    QString obj = "";
    for (QVector3D v : getVertices()) {
        obj += "v " + QString::number(v.x()) + " " + QString::number(v.y()) + " " + QString::number(v.z()) + "\n";
    }
    for (QVector2D uv : getUVMap()) {
        obj += "vt " + QString::number(uv.x()) + " " + QString::number(uv.y()) + "\n";
    }
    for (QVector3D vn : getNormals()) {
        obj += "vn " + QString::number(vn.x()) + " " + QString::number(vn.y()) + " " + QString::number(vn.z()) + "\n";
    }
    obj += "\nmtllib " + name + ".mtl\n";
    for (int i = 0; i < getGroupCount(); i++) {
        obj += "\ng " + QString::number(i) + "\n";
        obj += "usemtl mtl-" + QString::number(getTextures().at(i)) + "\n";
        for (QVector3D f : getGroup(i)) {
            obj += "f " + QString::number(f.x() + 1) + "/" + QString::number(f.x() + 1) + "/" +
                   QString::number(f.x() + 1) + " " + QString::number(f.y() + 1) + "/" + QString::number(f.y() + 1) +
                   "/" + QString::number(f.y() + 1) + " " + QString::number(f.z() + 1) + "/" +
                   QString::number(f.z() + 1) + "/" + QString::number(f.z() + 1) + "\n";
        }
    }
    QFile file(directory + name + ".obj");
    file.open(QIODevice::WriteOnly);
    file.write(obj.toLocal8Bit());
    file.close();

    QString mtl = "";
    for (int t : getTextures()) {
        mtl += "newmtl mtl-" + QString::number(t) + "\n";
        mtl += "Ns 10\nKa 0 0 0\nKd 0 0 0\nKs 0 0 0\n";
        mtl += "map_Kd " + QString::number(t) + ".png\n";
    }

    QFile file2(directory + name + ".mtl");
    file2.open(QIODevice::WriteOnly);
    file2.write(mtl.toLocal8Bit());
    file2.close();
    return 1;
}

int OnexNStgData::onReplace(QString directory) {
    return 0;
}

OnexNStgData::~OnexNStgData() {
}
