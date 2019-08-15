#include "ObjConverter.h"
#include <QDebug>
#include <QStringList>
#include <QtEndian>

ObjConverter::ObjConverter() {
    this->littleEndianConverter = new LittleEndianConverter();
}

Model *ObjConverter::fromObj(const QString &obj) {
    QVector<QVector3D> fileVertices;
    QVector<QVector3D> fileNormals;
    QVector<QVector2D> fileUV;
    QVector<Face> faces;
    QVector<int> textures;
    int group = -1;
    int object = -1;
    QStringList lines = obj.split("\n");
    for (const QString &line : lines) {
        QStringList parts = line.split(" ", QString::SplitBehavior::SkipEmptyParts);
        if (!parts.empty()) {
            if (parts[0] == "v") {
                fileVertices.append(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            } else if (parts[0] == "vn") {
                fileNormals.append(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            } else if (parts[0] == "vt") {
                fileUV.append(QVector2D(parts[1].toFloat(), parts[2].toFloat()));
            } else if (parts[0] == "f") {
                QStringList faceOneInfos = parts[1].split("/");
                QStringList faceTwoInfos = parts[2].split("/");
                QStringList faceThreeInfos = parts[3].split("/");
                Face f;
                if (object == -1)
                    object = 0;
                if (group == -1)
                    group = 0;
                f.object = object;
                f.group = group;
                f.v = QVector3D(faceOneInfos[0].toFloat() - 1, faceTwoInfos[0].toFloat() - 1,
                                faceThreeInfos[0].toFloat() - 1);
                f.vt = QVector3D(faceOneInfos[1].toFloat() - 1, faceTwoInfos[1].toFloat() - 1,
                                 faceThreeInfos[1].toFloat() - 1);
                f.vn = QVector3D(faceOneInfos[2].toFloat() - 1, faceTwoInfos[2].toFloat() - 1,
                                 faceThreeInfos[2].toFloat() - 1);
                faces.append(f);
            } else if (parts[0] == "usemtl") {
                QStringList texturemtl = parts[1].split("mtl-", QString::SplitBehavior::SkipEmptyParts);
                textures.append(texturemtl.at(0).toInt());
                group++;
            } else if (parts[0] == "o") {
                object++;
            }
        }
    }
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> uv;
    vertices.resize(fileVertices.size());
    normals.resize(fileVertices.size());
    uv.resize(fileVertices.size());
    for (auto &face : faces) {
        if ((!normals[face.v.x()].isNull() && normals[face.v.x()] != fileNormals[face.vn.x()]) ||
            (!uv[face.v.x()].isNull() && uv[face.v.x()] != fileUV[face.vt.x()])) {
            vertices.append(fileVertices[face.v.x()]);
            normals.append(fileNormals[face.vn.x()]);
            uv.append(fileUV[face.vt.x()]);
            face.v.setX(vertices.size() - 1);
            face.vn.setX(normals.size() - 1);
            face.vt.setX(uv.size() - 1);
        } else {
            vertices[face.v.x()] = fileVertices[face.v.x()];
            normals[face.v.x()] = fileNormals[face.vn.x()];
            uv[face.v.x()] = fileUV[face.vt.x()];
        }
        if ((!normals[face.v.y()].isNull() && normals[face.v.y()] != fileNormals[face.vn.y()]) ||
            (!uv[face.v.y()].isNull() && uv[face.v.y()] != fileUV[face.vt.y()])) {
            vertices.append(fileVertices[face.v.y()]);
            normals.append(fileNormals[face.vn.y()]);
            uv.append(fileUV[face.vt.y()]);
            face.v.setY(vertices.size() - 1);
            face.vn.setY(normals.size() - 1);
            face.vt.setY(uv.size() - 1);
        } else {
            vertices[face.v.y()] = fileVertices[face.v.y()];
            normals[face.v.y()] = fileNormals[face.vn.y()];
            uv[face.v.y()] = fileUV[face.vt.y()];
        }
        if ((!normals[face.v.z()].isNull() && normals[face.v.z()] != fileNormals[face.vn.z()]) ||
            (!uv[face.v.z()].isNull() && uv[face.v.z()] != fileUV[face.vt.z()])) {
            vertices.append(fileVertices[face.v.z()]);
            normals.append(fileNormals[face.vn.z()]);
            uv.append(fileUV[face.vt.z()]);
            face.v.setZ(vertices.size() - 1);
            face.vn.setZ(normals.size() - 1);
            face.vt.setZ(uv.size() - 1);
        } else {
            vertices[face.v.z()] = fileVertices[face.v.z()];
            normals[face.v.z()] = fileNormals[face.vn.z()];
            uv[face.v.z()] = fileUV[face.vt.z()];
        }
    }
    Model *model = new Model();
    model->vertices = vertices;
    model->normals = normals;
    model->uv = uv;
    model->uvScale = 0.0005;
    for (int i = 0; i <= object; i++) {
        ModelObject mo;
        mo.position = QVector3D(0, 0, 0);
        mo.rotation = QVector4D(0, 0, 0, 0);
        mo.scale = QVector3D(1, 1, 1);
        model->objects.append(mo);
    }
    for (int i = 0; i <= group; i++) {
        ModelGroup mg;
        mg.number = i;
        mg.texture = textures[i];
        model->groups.append(mg);
    }
    for (auto &face : faces) {
        model->groups[face.group].faces.append(face.v);
        if (!model->objects[face.object].groups.contains(face.group))
            model->objects[face.object].groups.append(face.group);
    }
    return model;
}

QStringList ObjConverter::toObj(Model *model, QString name) {
    QStringList list;
    list.append(generateObjFile(model, name));
    list.append(generateMtlFile(model));
    return list;
}

QString ObjConverter::generateObjFile(Model *model, const QString &name) {
    QString obj = "";
    obj += "mtllib " + name + ".mtl\n\n";
    for (QVector3D v : model->vertices) {
        obj += "v " + QString::number(v.x()) + " " + QString::number(v.y()) + " " + QString::number(v.z()) + "\n";
    }
    for (QVector2D p : model->uv) {
        obj += "vt " + QString::number(p.x()) + " " + QString::number(p.y()) + "\n";
    }
    for (QVector3D vn : model->normals) {
        obj += "vn " + QString::number(vn.x()) + " " + QString::number(vn.y()) + " " + QString::number(vn.z()) + "\n";
    }
    for (int o = 0; o < model->objects.size(); o++) {
        obj += "\no " + QString::number(o) + "\n";
        for (int g = 0; g < model->objects[o].groups.size(); g++) {
            obj += "usemtl mtl-" + QString::number(model->groups[model->objects[o].groups[g]].texture) + "\n";
            for (QVector3D f : model->groups[model->objects[o].groups[g]].faces) {
                obj += "f " + QString::number(f.x() + 1) + "/" + QString::number(f.x() + 1) + "/" +
                       QString::number(f.x() + 1) + " " + QString::number(f.y() + 1) + "/" +
                       QString::number(f.y() + 1) + "/" + QString::number(f.y() + 1) + " " +
                       QString::number(f.z() + 1) + "/" + QString::number(f.z() + 1) + "/" +
                       QString::number(f.z() + 1) + "\n";
            }
        }
    }
    return obj;
}

QString ObjConverter::generateMtlFile(Model *model) {
    QString mtl = "";
    for (auto &group : model->groups) {
        mtl += "newmtl mtl-" + QString::number(group.texture) + "\n";
        mtl += "Ns 10\nKa 0 0 0\nKd 0 0 0\nKs 0 0 0\n";
        mtl += "map_Kd " + QString::number(group.texture) + ".png\n";
    }
    return mtl;
}
