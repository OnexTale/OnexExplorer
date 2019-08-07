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

    for (int i = 0; i < model->objects.size(); i++) {
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-x-Position", model->objects[i].position.x()),
                &QLineEdit::textChanged, [=](const QString &value) { setXPosition(i, value.toFloat()); });
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-y-Position", model->objects[i].position.y()),
                &QLineEdit::textChanged, [=](const QString &value) { setYPosition(i, value.toFloat()); });
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-z-Position", model->objects[i].position.z()),
                &QLineEdit::textChanged, [=](const QString &value) { setZPosition(i, value.toFloat()); });

        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-x-Rotation", model->objects[i].rotation.x()),
                &QLineEdit::textChanged, [=](const QString &value) { setXRotation(i, value.toFloat()); });
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-y-Rotation", model->objects[i].rotation.y()),
                &QLineEdit::textChanged, [=](const QString &value) { setYRotation(i, value.toFloat()); });
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-z-Rotation", model->objects[i].rotation.z()),
                &QLineEdit::textChanged, [=](const QString &value) { setZRotation(i, value.toFloat()); });
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-w-Rotation", model->objects[i].rotation.w()),
                &QLineEdit::textChanged, [=](const QString &value) { setWRotation(i, value.toFloat()); });

        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-x-Scale", model->objects[i].scale.x()),
                &QLineEdit::textChanged, [=](const QString &value) { setXScale(i, value.toFloat()); });
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-y-Scale", model->objects[i].scale.y()),
                &QLineEdit::textChanged, [=](const QString &value) { setYScale(i, value.toFloat()); });
        connect(infos->addFloatLineEdit("O-" + QString::number(i) + "-z-Scale", model->objects[i].scale.z()),
                &QLineEdit::textChanged, [=](const QString &value) { setZScale(i, value.toFloat()); });
    }

    for (int i = 0; i < model->groups.size(); i++) {
        connect(infos->addIntLineEdit("Texture-" + QString::number(i), model->groups[i].texture),
                &QLineEdit::textChanged, [=](const QString &newValue) { setTexture(i, newValue.toInt()); });
    }

    return infos;
}

int OnexNStgData::onExport(QString directory) {
    QString path = "";
    if (!directory.endsWith(".obj"))
        path = directory + this->name + ".obj";
    else
        path = directory;

    if (model == nullptr) {
        NosModelConverter *nmc = new NosModelConverter();
        model = nmc->fromBinary(content);
    }

    ObjConverter *ojc = new ObjConverter();
    QStringList obj = ojc->toObj(model, name);

    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(obj.at(0).toLocal8Bit());
    file.close();

    QFile file2(path.split(".").at(0) + ".mtl");
    file2.open(QIODevice::WriteOnly);
    file2.write(obj.at(1).toLocal8Bit());
    file2.close();

    return 1;
}

int OnexNStgData::onReplace(QString directory) {
    QString path;
    if (!directory.endsWith(".obj"))
        path = directory + this->getName() + ".obj";
    else
        path = directory;

    if (!QFile(path).exists()) {
        QMessageBox::critical(NULL, "Woops", "Missing " + path);
        return 0;
    }

    QFile file(path);
    QString obj = "";
    if (file.open(QIODevice::ReadOnly))
        obj = QString::fromLocal8Bit(file.readAll());
    else {
        QMessageBox::critical(NULL, "Woops", "Couldn't open " + path);
        return 0;
    }

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

void OnexNStgData::setXPosition(int index, float x) {
    model->objects[index].position.setX(x);
    emit changeSignal("O-" + QString::number(index) + "-x-Position", x);
}

void OnexNStgData::setYPosition(int index, float y) {
    model->objects[index].position.setY(y);
    emit changeSignal("O-" + QString::number(index) + "-y-Position", y);
}

void OnexNStgData::setZPosition(int index, float z) {
    model->objects[index].position.setZ(z);
    emit changeSignal("O-" + QString::number(index) + "-z-Position", z);
}

void OnexNStgData::setXRotation(int index, float x) {
    model->objects[index].rotation.setX(x);
    emit changeSignal("O-" + QString::number(index) + "-x-Rotation", x);
}
void OnexNStgData::setYRotation(int index, float y) {
    model->objects[index].rotation.setY(y);
    emit changeSignal("O-" + QString::number(index) + "-y-Rotation", y);
}
void OnexNStgData::setZRotation(int index, float z) {
    model->objects[index].rotation.setZ(z);
    emit changeSignal("O-" + QString::number(index) + "-z-Rotation", z);
}
void OnexNStgData::setWRotation(int index, float w) {
    model->objects[index].rotation.setW(w);
    emit changeSignal("O-" + QString::number(index) + "-w-Rotation", w);
}

void OnexNStgData::setXScale(int index, float x) {
    model->objects[index].scale.setX(x);
    emit changeSignal("O-" + QString::number(index) + "-x-Scale", x);
}

void OnexNStgData::setYScale(int index, float y) {
    model->objects[index].scale.setY(y);
    emit changeSignal("O-" + QString::number(index) + "-y-Scale", y);
}

void OnexNStgData::setZScale(int index, float z) {
    model->objects[index].scale.setZ(z);
    emit changeSignal("O-" + QString::number(index) + "-z-Scale", z);
}

void OnexNStgData::setTexture(int index, int texture) {
    model->groups[index].texture = texture;
    emit changeSignal("Texture-" + QString::number(index), texture);
}

QString OnexNStgData::getExportExtension() {
    return ".obj";
}

OnexNStgData::~OnexNStgData() {
}
