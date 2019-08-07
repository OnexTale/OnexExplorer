#include "OnexNStgData.h"
#include "../Previews/SingleModelPreview.h"

OnexNStgData::OnexNStgData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
    : OnexTreeZlibItem(header, name, content, opener, id, creationDate, compressed) {
    model = nullptr;
}

QWidget *OnexNStgData::getPreview() {
    if (!hasParent())
        return nullptr;

    if (model == nullptr) {
        NosModelConverter *nmc = new NosModelConverter();
        model = nmc->fromBinary(content);
    }

    QVector<QVector3D> faces;
    for (ModelGroup mg : model->groups) {
        faces.append(mg.faces);
    }

    SingleModelPreview *modelPreview = new SingleModelPreview(model);
    connect(this, SIGNAL(replaceSignal(Model *)), modelPreview, SLOT(onReplaced(Model *)));

    return modelPreview;
}

FileInfo *OnexNStgData::getInfos() {
    if (!hasParent())
        return nullptr;

    FileInfo *infos = OnexTreeZlibItem::getInfos();

    connect(infos->addFloatLineEdit("x-Position", model->objects[0].position.x()), &QLineEdit::textChanged,
            [=](const QString &value) { setXPosition(value.toFloat()); });
    connect(infos->addFloatLineEdit("y-Position", model->objects[0].position.y()), &QLineEdit::textChanged,
            [=](const QString &value) { setYPosition(value.toFloat()); });
    connect(infos->addFloatLineEdit("z-Position", model->objects[0].position.z()), &QLineEdit::textChanged,
            [=](const QString &value) { setZPosition(value.toFloat()); });

    connect(infos->addFloatLineEdit("x-Rotation", model->objects[0].rotation.x()), &QLineEdit::textChanged,
            [=](const QString &value) { setXRotation(value.toFloat()); });
    connect(infos->addFloatLineEdit("y-Rotation", model->objects[0].rotation.y()), &QLineEdit::textChanged,
            [=](const QString &value) { setYRotation(value.toFloat()); });
    connect(infos->addFloatLineEdit("z-Rotation", model->objects[0].rotation.z()), &QLineEdit::textChanged,
            [=](const QString &value) { setZRotation(value.toFloat()); });
    connect(infos->addFloatLineEdit("w-Rotation", model->objects[0].rotation.w()), &QLineEdit::textChanged,
            [=](const QString &value) { setWRotation(value.toFloat()); });

    connect(infos->addFloatLineEdit("x-Scale", model->objects[0].scale.x()), &QLineEdit::textChanged,
            [=](const QString &value) { setXScale(value.toFloat()); });
    connect(infos->addFloatLineEdit("y-Scale", model->objects[0].scale.y()), &QLineEdit::textChanged,
            [=](const QString &value) { setYScale(value.toFloat()); });
    connect(infos->addFloatLineEdit("z-Scale", model->objects[0].scale.z()), &QLineEdit::textChanged,
            [=](const QString &value) { setZScale(value.toFloat()); });

    for (int i = 0; i < model->groups.size(); i++) {
        connect(infos->addIntLineEdit("Texture-" + QString::number(i), model->groups[i].texture),
                &QLineEdit::textChanged, [=](const QString &newValue) { setTexture(i, newValue.toInt()); });
    }

    return infos;
}

int OnexNStgData::onExport(QString directory) {
    if (model == nullptr) {
        NosModelConverter *nmc = new NosModelConverter();
        model = nmc->fromBinary(content);
    }

    ObjConverter *ojc = new ObjConverter();
    QStringList obj = ojc->toObj(model, name);

    QFile file(directory + name + ".obj");
    file.open(QIODevice::WriteOnly);
    file.write(obj.at(0).toLocal8Bit());
    file.close();

    QFile file2(directory + name + ".mtl");
    file2.open(QIODevice::WriteOnly);
    file2.write(obj.at(1).toLocal8Bit());
    file2.close();

    return 1;
}

int OnexNStgData::onReplace(QString directory) {
    QString path = directory + this->name + ".obj";
    if (!QFile(path).exists()) {
        QMessageBox::critical(NULL, "Woops", "Missing " + path);
        return 0;
    }

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QString obj = QString::fromLocal8Bit(file.readAll());

    ObjConverter *oc = new ObjConverter();
    model = oc->fromObj(obj);

    emit replaceSignal(this->model);

    FileInfo *newInfo = getInfos();
    emit replaceInfo(newInfo);

    return 1;
}

QByteArray OnexNStgData::getContent() {
    if (!hasParent() || model == nullptr)
        return content;

    QByteArray newContent = content.mid(0, 0x30);

    NosModelConverter *nmc = new NosModelConverter();

    newContent.append(nmc->toBinary(model, 0.00005));

    content = newContent;
    return content;
}

void OnexNStgData::setXPosition(float x) {
    model->objects[0].position.setX(x);
    emit changeSignal("x-Position", x);
}

void OnexNStgData::setYPosition(float y) {
    model->objects[0].position.setY(y);
    emit changeSignal("y-Position", y);
}

void OnexNStgData::setZPosition(float z) {
    model->objects[0].position.setZ(z);
    emit changeSignal("z-Position", z);
}

void OnexNStgData::setXRotation(float x) {
    model->objects[0].rotation.setX(x);
    emit changeSignal("x-Rotation", x);
}
void OnexNStgData::setYRotation(float y) {
    model->objects[0].rotation.setY(y);
    emit changeSignal("y-Rotation", y);
}
void OnexNStgData::setZRotation(float z) {
    model->objects[0].rotation.setZ(z);
    emit changeSignal("z-Rotation", z);
}
void OnexNStgData::setWRotation(float w) {
    model->objects[0].rotation.setW(w);
    emit changeSignal("w-Rotation", w);
}

void OnexNStgData::setXScale(float x) {
    model->objects[0].scale.setX(x);
    emit changeSignal("x-Scale", x);
}

void OnexNStgData::setYScale(float y) {
    model->objects[0].scale.setY(y);
    emit changeSignal("y-Scale", y);
}

void OnexNStgData::setZScale(float z) {
    model->objects[0].scale.setZ(z);
    emit changeSignal("z-Scale", z);
}

void OnexNStgData::setTexture(int index, int texture) {
    model->groups[index].texture = texture;
    emit changeSignal("Texture-" + QString::number(index), texture);
}

OnexNStgData::~OnexNStgData() {
}
