#include "NosModelConverter.h"
#include <QDebug>
#include <QStringList>
#include <QtEndian>

NosModelConverter::NosModelConverter() {
}

Model *NosModelConverter::fromBinary(QByteArray obj) {
    Model *model = new Model();

    float uvScale = fromLittleEndianToFloat(obj.mid(0x30, 4));
    int verticeCount = fromLittleEndianToShort(obj.mid(0x34, 2));
    int offset = 0x36;
    model->vertices = readNosVertices(obj, offset, verticeCount);
    model->uv = readNosUV(obj, offset, verticeCount, uvScale);
    model->normals = readNosNormals(obj, offset, verticeCount);
    QVector<ModelGroup> faces = readNosFaces(obj, offset);

    model = readModelConstruction(obj, offset, model, faces);

    return model;
}

QByteArray NosModelConverter::toBinary(Model *model, float uvScale) {
    QByteArray newContent;
    newContent.append(fromFloatToLittleEndian(uvScale));
    newContent.append(fromShortToLittleEndian(model->vertices.size()));

    for (int i = 0; i < model->vertices.size(); i++) {
        newContent.append(fromFloatToLittleEndian(model->vertices[i].x()));
        newContent.append(fromFloatToLittleEndian(model->vertices[i].y()));
        newContent.append(fromFloatToLittleEndian(model->vertices[i].z()));
    }
    for (int i = 0; i < model->uv.size(); i++) {
        float u = model->uv[i].x();
        if ((int)u != 1 && (int)u != -1)
            u -= (int)u;
        u /= uvScale;
        float v = model->uv[i].y();
        if ((int)v != 1 && (int)v != -1)
            v -= (int)v;
        v = (1.0 - v) / uvScale;
        newContent.append(fromShortToLittleEndian(u));
        newContent.append(fromShortToLittleEndian(v));
    }
    for (int i = 0; i < model->normals.size(); i++) {
        uint8_t x = model->normals[i].x() * 0x7F;
        uint8_t y = model->normals[i].y() * 0x7F;
        uint8_t z = model->normals[i].z() * 0x7F;
        newContent.append(x);
        newContent.append(y);
        newContent.append(z);
    }

    newContent.append(fromShortToLittleEndian(model->groups.size()));
    for (int g = 0; g < model->groups.size(); g++) {
        newContent.append(fromShortToLittleEndian(model->groups[g].faces.size() * 3)); // GroupSize
        for (int j = 0; j < model->groups[g].faces.size(); j++) {
            newContent.append(fromShortToLittleEndian(model->groups[g].faces[j].x()));
            newContent.append(fromShortToLittleEndian(model->groups[g].faces[j].y()));
            newContent.append(fromShortToLittleEndian(model->groups[g].faces[j].z()));
        }
    }

    newContent.append(fromShortToLittleEndian(model->objects.size())); // ObjectCount
    for (int m = 0; m < model->objects.size(); m++) {
        newContent.append(fromFloatToLittleEndian(model->objects[m].position.x())); // x
        newContent.append(fromFloatToLittleEndian(model->objects[m].position.y())); // y
        newContent.append(fromFloatToLittleEndian(model->objects[m].position.z())); // z

        newContent.append(fromShortToLittleEndian(model->objects[m].rotation.x() * 0x7FFF)); // x rotation
        newContent.append(fromShortToLittleEndian(model->objects[m].rotation.y() * 0x7FFF)); // y rotation
        newContent.append(fromShortToLittleEndian(model->objects[m].rotation.z() * 0x7FFF)); // z rotation
        newContent.append(fromShortToLittleEndian(model->objects[m].rotation.w() * 0x7FFF)); // w rotation

        newContent.append(fromFloatToLittleEndian(model->objects[m].scale.x())); // x scale
        newContent.append(fromFloatToLittleEndian(model->objects[m].scale.y())); // y scale
        newContent.append(fromFloatToLittleEndian(model->objects[m].scale.z())); // z scale

        newContent.append(fromShortToLittleEndian(model->objects[m].animationPositions.size())); // translationFrameCount
        for (int a = 0; a < model->objects[m].animationPositions.size(); a++) {
            newContent.append(fromShortToLittleEndian(model->objects[m].animationPositions[a].timestamp));
            newContent.append(fromFloatToLittleEndian(model->objects[m].animationPositions[a].position.x()));
            newContent.append(fromFloatToLittleEndian(model->objects[m].animationPositions[a].position.y()));
            newContent.append(fromFloatToLittleEndian(model->objects[m].animationPositions[a].position.z()));
        }
        newContent.append(fromShortToLittleEndian(model->objects[m].animationRotations.size())); // rotationFrameCount
        for (int a = 0; a < model->objects[m].animationRotations.size(); a++) {
            newContent.append(fromShortToLittleEndian(model->objects[m].animationRotations[a].timestamp));
            newContent.append(fromShortToLittleEndian(model->objects[m].animationRotations[a].rotation.x() * 0x7FFF));
            newContent.append(fromShortToLittleEndian(model->objects[m].animationRotations[a].rotation.y() * 0x7FFF));
            newContent.append(fromShortToLittleEndian(model->objects[m].animationRotations[a].rotation.z() * 0x7FFF));
            newContent.append(fromShortToLittleEndian(model->objects[m].animationRotations[a].rotation.w() * 0x7FFF));
        }
        newContent.append(fromShortToLittleEndian(model->objects[m].animationScales.size())); // scaleFrameCount
        for (int a = 0; a < model->objects[m].animationScales.size(); a++) {
            newContent.append(fromShortToLittleEndian(model->objects[m].animationScales[a].timestamp));
            newContent.append(fromFloatToLittleEndian(model->objects[m].animationScales[a].scale.x()));
            newContent.append(fromFloatToLittleEndian(model->objects[m].animationScales[a].scale.y()));
            newContent.append(fromFloatToLittleEndian(model->objects[m].animationScales[a].scale.z()));
        }

        newContent.append(fromShortToLittleEndian(model->objects[m].groups.size()));
        for (int i = 0; i < model->objects[m].groups.size(); i++) {
            newContent.append(fromIntToLittleEndian(model->groups[model->objects[m].groups[i]].texture));
            newContent.append((char)0);                                                                    // bool
            newContent.append(fromShortToLittleEndian(model->groups[model->objects[m].groups[i]].number)); // group
        }
        newContent.append(fromShortToLittleEndian(0));
    }

    return newContent;
}

QVector<QVector3D> NosModelConverter::readNosVertices(QByteArray obj, int &offset, int verticeCount) {
    QVector<QVector3D> vertices;
    for (int i = 0; i < verticeCount; i++) {
        float x = fromLittleEndianToFloat(obj.mid(offset, 4));
        float y = fromLittleEndianToFloat(obj.mid(offset + 4, 4));
        float z = fromLittleEndianToFloat(obj.mid(offset + 8, 4));
        vertices.append(QVector3D(x, y, z));
        offset += 12;
    }
    return vertices;
}
QVector<QVector3D> NosModelConverter::readNosNormals(QByteArray obj, int &offset, int verticeCount) {
    QVector<QVector3D> normals;
    for (int i = 0; i < verticeCount; i++) {
        float x = (float)obj.at(offset) / 0x7F;
        float y = (float)obj.at(offset + 1) / 0x7F;
        float z = (float)obj.at(offset + 2) / 0x7F;
        normals.append(QVector3D(x, y, z));
        offset += 3;
    }
    return normals;
}
QVector<QVector2D> NosModelConverter::readNosUV(QByteArray obj, int &offset, int verticeCount, float uvScale) {
    QVector<QVector2D> uv;
    for (int i = 0; i < verticeCount; i++) {
        float u = uvScale * fromLittleEndianToShort(obj.mid(offset, 2));
        float v = 1.0 - (uvScale * fromLittleEndianToShort(obj.mid(offset + 2, 2)));
        uv.append(QVector2D(u, v));
        offset += 4;
    }
    return uv;
}
QVector<ModelGroup> NosModelConverter::readNosFaces(QByteArray obj, int &offset) {
    QVector<ModelGroup> groups;
    int groupCount = fromLittleEndianToShort(obj.mid(offset, 2));
    offset += 2;

    for (int g = 0; g < groupCount; g++) {
        ModelGroup group;
        group.number = g;
        int points = (uint16_t)fromLittleEndianToShort(obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < points / 3; i++) {
            int p1 = fromLittleEndianToShort(obj.mid(offset, 2));
            int p2 = fromLittleEndianToShort(obj.mid(offset + 2, 2));
            int p3 = fromLittleEndianToShort(obj.mid(offset + 4, 2));
            group.faces.append(QVector3D(p1, p2, p3));
            offset += 6;
        }
        groups.append(group);
    }
    return groups;
}

Model *NosModelConverter::readModelConstruction(QByteArray obj, int &offset, Model *model, QVector<ModelGroup> groups) {
    int parts = fromLittleEndianToShort(obj.mid(offset, 2));
    offset += 2;

    for (int p = 0; p < parts; p++) {
        ModelObject modelPart;

        float x = fromLittleEndianToFloat(obj.mid(offset, 4));
        float y = fromLittleEndianToFloat(obj.mid(offset + 4, 4));
        float z = fromLittleEndianToFloat(obj.mid(offset + 8, 4));
        modelPart.position = QVector3D(x, y, z);
        offset += 12;

        float rotation_x = (float)fromLittleEndianToShort(obj.mid(offset, 2)) / 0x7FFF;
        float rotation_y = (float)fromLittleEndianToShort(obj.mid(offset + 2, 2)) / 0x7FFF;
        float rotation_z = (float)fromLittleEndianToShort(obj.mid(offset + 4, 2)) / 0x7FFF;
        float rotation_w = (float)fromLittleEndianToShort(obj.mid(offset + 6, 2)) / 0x7FFF;
        modelPart.rotation = QVector4D(rotation_x, rotation_y, rotation_z, rotation_w);
        offset += 8;

        float scale_x = fromLittleEndianToFloat(obj.mid(offset, 4));
        float scale_y = fromLittleEndianToFloat(obj.mid(offset + 4, 4));
        float scale_z = fromLittleEndianToFloat(obj.mid(offset + 8, 4));
        modelPart.scale = QVector3D(scale_x, scale_y, scale_z);
        offset += 12;

        int translationFrameCount = fromLittleEndianToShort(obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < translationFrameCount; i++) {
            short time = fromLittleEndianToShort(obj.mid(offset, 2));
            float frame_x = fromLittleEndianToFloat(obj.mid(offset + 2, 4));
            float frame_y = fromLittleEndianToFloat(obj.mid(offset + 6, 4));
            float frame_z = fromLittleEndianToFloat(obj.mid(offset + 10, 4));
            ModelAnimationPositionFrame aPos;
            aPos.timestamp = time;
            aPos.position = QVector3D(frame_x, frame_y, frame_z);
            modelPart.animationPositions.append(aPos);
            offset += 14;
        }
        int rotationFrameCount = fromLittleEndianToShort(obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < rotationFrameCount; i++) {
            short time = fromLittleEndianToShort(obj.mid(offset, 2));
            float frame_rotation_x = (float)fromLittleEndianToShort(obj.mid(offset + 2, 2)) / 0x7FFF;
            float frame_rotation_y = (float)fromLittleEndianToShort(obj.mid(offset + 4, 2)) / 0x7FFF;
            float frame_rotation_z = (float)fromLittleEndianToShort(obj.mid(offset + 6, 2)) / 0x7FFF;
            float frame_rotation_w = (float)fromLittleEndianToShort(obj.mid(offset + 8, 2)) / 0x7FFF;
            ModelAnimationRotationFrame aPos;
            aPos.timestamp = time;
            aPos.rotation = QVector4D(frame_rotation_x, frame_rotation_y, frame_rotation_z, frame_rotation_w);
            modelPart.animationRotations.append(aPos);
            offset += 10;
        }
        int scaleFrameCount = fromLittleEndianToShort(obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < scaleFrameCount; i++) {
            short time = fromLittleEndianToShort(obj.mid(offset, 2));
            float frame_scale_x = fromLittleEndianToFloat(obj.mid(offset + 2, 4));
            float frame_scale_y = fromLittleEndianToFloat(obj.mid(offset + 6, 4));
            float frame_scale_z = fromLittleEndianToFloat(obj.mid(offset + 10, 4));
            ModelAnimationScaleFrame aPos;
            aPos.timestamp = time;
            aPos.scale = QVector3D(frame_scale_x, frame_scale_y, frame_scale_z);
            modelPart.animationScales.append(aPos);
            offset += 14;
        }

        int c = fromLittleEndianToShort(obj.mid(offset, 2));
        offset += 2;
        for (int i = 0; i < c; i++) {
            int img = fromLittleEndianToInt(obj.mid(offset, 4));
            offset += 4;
            bool smooth = obj.at(offset); // maybe means something else
            offset++;
            int grp = fromLittleEndianToShort(obj.mid(offset, 2)); // equals i?
            groups[grp].texture = img;
            model->groups.append(groups[grp]);
            modelPart.groups.append(grp);
            offset += 2;
        }
        fromLittleEndianToShort(obj.mid(offset, 2)); // seems to be always 0?
        offset += 2;

        model->objects.append(modelPart);
    }

    return model;
}