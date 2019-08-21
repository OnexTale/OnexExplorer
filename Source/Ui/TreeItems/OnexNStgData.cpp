#include "OnexNStgData.h"
#include "../Previews/SingleModelPreview.h"

ObjConverter OnexNStgData::objConverter = ObjConverter();
NosModelConverter OnexNStgData::nosModelConverter = NosModelConverter();

OnexNStgData::OnexNStgData(QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
        : OnexTreeZlibItem(name, content, opener, id, creationDate, compressed) {
    model = nullptr;
}

OnexNStgData::~OnexNStgData() = default;

QWidget *OnexNStgData::getPreview() {
    if (!hasParent())
        return nullptr;
    if (model == nullptr) {
        model = nosModelConverter.fromBinary(content);
    }
    auto *modelPreview = new SingleModelPreview(model);
    connect(this, SIGNAL(replaceSignal(Model * )), modelPreview, SLOT(onReplaced(Model * )));

    auto *wrapper = new QWidget();
    wrapper->setLayout(new QGridLayout());
    wrapper->layout()->addWidget(modelPreview);
    return wrapper;
}

QByteArray OnexNStgData::getContent() {
    if (!hasParent() || model == nullptr)
        return content;
    QByteArray newContent = content.mid(0, 0x30);
    newContent.append(nosModelConverter.toBinary(model));
    content = newContent;
    return content;
}

int OnexNStgData::saveAsFile(const QString &path, QByteArray content) {
    if (model == nullptr) {
        model = nosModelConverter.fromBinary(this->content);
    }
    QStringList obj = objConverter.toObj(model, name);

    if (OnexTreeItem::saveAsFile(path, obj.at(0).toLocal8Bit()) == 0)
        return 0;
    if (OnexTreeItem::saveAsFile(path.split(".").at(0) + ".mtl", obj.at(1).toLocal8Bit()) == 0)
        return 0;
    return 1;
}

QString OnexNStgData::getExportExtension() {
    return ".obj";
}

int OnexNStgData::afterReplace(QByteArray content) {
    float scale = model->uvScale;
    model = objConverter.fromObj(content);
    model->uvScale = scale;

    emit replaceSignal(this->model);
    emit replaceInfo(generateInfos());
    return 1;
}

void OnexNStgData::setXPosition(int index, float x, bool update) {
    model->objects[index].position.setX(x);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-x-Position", x);
}

void OnexNStgData::setYPosition(int index, float y, bool update) {
    model->objects[index].position.setY(y);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-y-Position", y);
}

void OnexNStgData::setZPosition(int index, float z, bool update) {
    model->objects[index].position.setZ(z);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-z-Position", z);
}

void OnexNStgData::setXRotation(int index, float x, bool update) {
    model->objects[index].rotation.setX(x);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-x-Rotation", x);
}

void OnexNStgData::setYRotation(int index, float y, bool update) {
    model->objects[index].rotation.setY(y);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-y-Rotation", y);
}

void OnexNStgData::setZRotation(int index, float z, bool update) {
    model->objects[index].rotation.setZ(z);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-z-Rotation", z);
}

void OnexNStgData::setWRotation(int index, float w, bool update) {
    model->objects[index].rotation.setW(w);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-w-Rotation", w);
}

void OnexNStgData::setXScale(int index, float x, bool update) {
    model->objects[index].scale.setX(x);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-x-Scale", x);
}

void OnexNStgData::setYScale(int index, float y, bool update) {
    model->objects[index].scale.setY(y);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-y-Scale", y);
}

void OnexNStgData::setZScale(int index, float z, bool update) {
    model->objects[index].scale.setZ(z);
    if (update)
            emit changeSignal("O-" + QString::number(index) + "-z-Scale", z);
}

void OnexNStgData::setTexture(int index, int texture, bool update) {
    model->groups[index].texture = texture;
    if (update)
            emit changeSignal("Texture-" + QString::number(index), texture);
}

void OnexNStgData::setUVScale(float scale, bool update) {
    model->uvScale = scale;
    if (update)
            emit changeSignal("UV-Scale", scale);
}

FileInfo *OnexNStgData::generateInfos() {
    FileInfo *infos = OnexTreeZlibItem::generateInfos();
    for (int i = 0; i < model->objects.size(); i++) {
        connect(infos->addFloatLineEdit("UV-Scale", model->uvScale),
                &QLineEdit::textChanged, [=](const QString &value) { setUVScale(value.toFloat()); });
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
