#include "ObjConverter.h"
#include <QDebug>
#include <QStringList>
#include <QtEndian>

ObjConverter::ObjConverter() {
}

struct Face {
    int object;
    int group;
    QVector3D v;
    QVector3D vt;
    QVector3D vn;
};

Model *ObjConverter::fromObj(QString obj) {
    QVector<QVector3D> fileVertices;
    QVector<QVector3D> fileNormals;
    QVector<QVector2D> fileUV;
    QVector<Face> faces;
    QVector<int> textures;
    int group = -1;
    int object = -1;

    QStringList lines = obj.split("\n");
    for (QString line : lines) {
        QStringList parts = line.split(" ", QString::SplitBehavior::SkipEmptyParts);
        if (parts.size() > 0) {
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

    for (int i = 0; i < faces.size(); i++) {
        if ((!normals[faces[i].v.x()].isNull() && normals[faces[i].v.x()] != fileNormals[faces[i].vn.x()]) ||
            (!uv[faces[i].v.x()].isNull() && uv[faces[i].v.x()] != fileUV[faces[i].vt.x()])) {
            vertices.append(fileVertices[faces[i].v.x()]);
            normals.append(fileNormals[faces[i].vn.x()]);
            uv.append(fileUV[faces[i].vt.x()]);
            faces[i].v.setX(vertices.size() - 1);
            faces[i].vn.setX(normals.size() - 1);
            faces[i].vt.setX(uv.size() - 1);
        } else {
            vertices[faces[i].v.x()] = fileVertices[faces[i].v.x()];
            normals[faces[i].v.x()] = fileNormals[faces[i].vn.x()];
            uv[faces[i].v.x()] = fileUV[faces[i].vt.x()];
        }

        if ((!normals[faces[i].v.y()].isNull() && normals[faces[i].v.y()] != fileNormals[faces[i].vn.y()]) ||
            (!uv[faces[i].v.y()].isNull() && uv[faces[i].v.y()] != fileUV[faces[i].vt.y()])) {
            vertices.append(fileVertices[faces[i].v.y()]);
            normals.append(fileNormals[faces[i].vn.y()]);
            uv.append(fileUV[faces[i].vt.y()]);
            faces[i].v.setY(vertices.size() - 1);
            faces[i].vn.setY(normals.size() - 1);
            faces[i].vt.setY(uv.size() - 1);
        } else {
            vertices[faces[i].v.y()] = fileVertices[faces[i].v.y()];
            normals[faces[i].v.y()] = fileNormals[faces[i].vn.y()];
            uv[faces[i].v.y()] = fileUV[faces[i].vt.y()];
        }

        if ((!normals[faces[i].v.z()].isNull() && normals[faces[i].v.z()] != fileNormals[faces[i].vn.z()]) ||
            (!uv[faces[i].v.z()].isNull() && uv[faces[i].v.z()] != fileUV[faces[i].vt.z()])) {
            vertices.append(fileVertices[faces[i].v.z()]);
            normals.append(fileNormals[faces[i].vn.z()]);
            uv.append(fileUV[faces[i].vt.z()]);
            faces[i].v.setZ(vertices.size() - 1);
            faces[i].vn.setZ(normals.size() - 1);
            faces[i].vt.setZ(uv.size() - 1);
        } else {
            vertices[faces[i].v.z()] = fileVertices[faces[i].v.z()];
            normals[faces[i].v.z()] = fileNormals[faces[i].vn.z()];
            uv[faces[i].v.z()] = fileUV[faces[i].vt.z()];
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

    for (int i = 0; i < faces.size(); i++) {
        model->groups[faces[i].group].faces.append(faces[i].v);
        if (!model->objects[faces[i].object].groups.contains(faces[i].group))
            model->objects[faces[i].object].groups.append(faces[i].group);
    }

    return model;
}

QStringList ObjConverter::toObj(Model *model, QString name) {
    QStringList list;
    list.append(generateObjFile(model, name));
    list.append(generateMtlFile(model));
    return list;
}

QString ObjConverter::generateObjFile(Model *model, QString name) {
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
    for (int g = 0; g < model->groups.size(); g++) {
        mtl += "newmtl mtl-" + QString::number(model->groups[g].texture) + "\n";
        mtl += "Ns 10\nKa 0 0 0\nKd 0 0 0\nKs 0 0 0\n";
        mtl += "map_Kd " + QString::number(model->groups[g].texture) + ".png\n";
    }
    return mtl;
}
