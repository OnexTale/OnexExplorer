#include "OnexNStgData.h"
#include "../Previews/SingleModelPreview.h"

OnexNStgData::OnexNStgData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                           int creationDate, bool compressed)
    : OnexTreeZlibItem(header, name, content, opener, id, creationDate, compressed) {
    NosModelConverter *nmc = new NosModelConverter();
    model = nmc->fromBinary(content);
}

QWidget *OnexNStgData::getPreview() {
    if (!hasParent())
        return nullptr;

    NosModelConverter *nmc = new NosModelConverter();
    model = nmc->fromBinary(content);

    QVector<QVector3D> faces;
    for (ModelGroup mg : model.groups) {
        faces.append(mg.faces);
    }

    SingleModelPreview *modelPreview = new SingleModelPreview(model);
    connect(this, SIGNAL(replaceSignal(Model)), modelPreview, SLOT(onReplaced(Model)));
    
    return modelPreview;
}

QWidget *OnexNStgData::getInfos() {
    if (!hasParent())
        return nullptr;

    QWidget *w = OnexTreeZlibItem::getInfos();
    QGridLayout *infoLayout = static_cast<QGridLayout *>(w->layout());

    QLabel *xLabel = new QLabel("x-Position");
    infoLayout->addWidget(xLabel, 7, 0);
    QLineEdit *xIn = new QLineEdit(QString::number(model.objects[0].position.x()));
    connect(xIn, &QLineEdit::textChanged, [=](const QString &value) { setXPosition(value.toFloat()); });
    infoLayout->addWidget(xIn, 7, 1);

    QLabel *yLabel = new QLabel("y-Position");
    infoLayout->addWidget(yLabel, 8, 0);
    QLineEdit *yIn = new QLineEdit(QString::number(model.objects[0].position.y()));
    connect(yIn, &QLineEdit::textChanged, [=](const QString &value) { setYPosition(value.toFloat()); });
    infoLayout->addWidget(yIn, 8, 1);

    QLabel *zLabel = new QLabel("z-Position");
    infoLayout->addWidget(zLabel, 9, 0);
    QLineEdit *zIn = new QLineEdit(QString::number(model.objects[0].position.z()));
    connect(zIn, &QLineEdit::textChanged, [=](const QString &value) { setZPosition(value.toFloat()); });
    infoLayout->addWidget(zIn, 9, 1);

    QLabel *xRotationLabel = new QLabel("x-Rotation");
    infoLayout->addWidget(xRotationLabel, 10, 0);
    QLineEdit *xRotationIn = new QLineEdit(QString::number(model.objects[0].rotation.x()));
    connect(xRotationIn, &QLineEdit::textChanged, [=](const QString &value) { setXRotation(value.toFloat()); });
    infoLayout->addWidget(xRotationIn, 10, 1);

    QLabel *yRotationLabel = new QLabel("y-Rotation");
    infoLayout->addWidget(yRotationLabel, 11, 0);
    QLineEdit *yRotationIn = new QLineEdit(QString::number(model.objects[0].rotation.y()));
    connect(yRotationIn, &QLineEdit::textChanged, [=](const QString &value) { setYRotation(value.toFloat()); });
    infoLayout->addWidget(yRotationIn, 11, 1);

    QLabel *zRotationLabel = new QLabel("z-Rotation");
    infoLayout->addWidget(zRotationLabel, 12, 0);
    QLineEdit *zRotationIn = new QLineEdit(QString::number(model.objects[0].rotation.z()));
    connect(zRotationIn, &QLineEdit::textChanged, [=](const QString &value) { setZRotation(value.toFloat()); });
    infoLayout->addWidget(zRotationIn, 12, 1);

    QLabel *wRotationLabel = new QLabel("w-Rotation");
    infoLayout->addWidget(wRotationLabel, 13, 0);
    QLineEdit *wRotationIn = new QLineEdit(QString::number(model.objects[0].rotation.w()));
    connect(wRotationIn, &QLineEdit::textChanged, [=](const QString &value) { setWRotation(value.toFloat()); });
    infoLayout->addWidget(wRotationIn, 13, 1);

    QLabel *xScaleLabel = new QLabel("x-Scale");
    infoLayout->addWidget(xScaleLabel, 14, 0);
    QLineEdit *xScaleIn = new QLineEdit(QString::number(model.objects[0].scale.x()));
    connect(xScaleIn, &QLineEdit::textChanged, [=](const QString &value) { setXScale(value.toFloat()); });
    infoLayout->addWidget(xScaleIn, 14, 1);

    QLabel *yScaleLabel = new QLabel("y-Scale");
    infoLayout->addWidget(yScaleLabel, 15, 0);
    QLineEdit *yScaleIn = new QLineEdit(QString::number(model.objects[0].scale.y()));
    connect(yScaleIn, &QLineEdit::textChanged, [=](const QString &value) { setYScale(value.toFloat()); });
    infoLayout->addWidget(yScaleIn, 15, 1);

    QLabel *zScaleLabel = new QLabel("z-Scale");
    infoLayout->addWidget(zScaleLabel, 16, 0);
    QLineEdit *zScaleIn = new QLineEdit(QString::number(model.objects[0].scale.z()));
    connect(zScaleIn, &QLineEdit::textChanged, [=](const QString &value) { setZScale(value.toFloat()); });
    infoLayout->addWidget(zScaleIn, 16, 1);

    QLabel *texturesLabel = new QLabel("Textures");
    infoLayout->addWidget(texturesLabel, 17, 0);

    for (int i = 0; i < model.groups.size(); i++) {
        QLabel *textureLabel = new QLabel("Texture-" + QString::number(i));
        infoLayout->addWidget(textureLabel, 18 + i, 0);
        QLineEdit *textureIn = new QLineEdit(QString::number(model.groups[i].texture));
        connect(textureIn, &QLineEdit::textChanged, [=](const QString &newValue) { setTexture(i, newValue.toInt()); });
        infoLayout->addWidget(textureIn, 18 + i, 1);
    }

    return w;
}

int OnexNStgData::onExport(QString directory) {
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

    return 1;
}

QByteArray OnexNStgData::getContent() {
    if (!hasParent())
        return content;

    QByteArray newContent = content.mid(0, 0x30);

    NosModelConverter *nmc = new NosModelConverter();

    newContent.append(nmc->toBinary(model, 0.00005));

    content = newContent;
    return content;
}

void OnexNStgData::setXPosition(float x) {
    model.objects[0].position.setX(x);
}

void OnexNStgData::setYPosition(float y) {
    model.objects[0].position.setY(y);
}

void OnexNStgData::setZPosition(float z) {
    model.objects[0].position.setZ(z);
}

void OnexNStgData::setXRotation(float x) {
    model.objects[0].rotation.setX(x);
}
void OnexNStgData::setYRotation(float y) {
    model.objects[0].rotation.setY(y);
}
void OnexNStgData::setZRotation(float z) {
    model.objects[0].rotation.setZ(z);
}
void OnexNStgData::setWRotation(float w) {
    model.objects[0].rotation.setW(w);
}

void OnexNStgData::setXScale(float x) {
    model.objects[0].scale.setX(x);
}

void OnexNStgData::setYScale(float y) {
    model.objects[0].scale.setY(y);
}

void OnexNStgData::setZScale(float z) {
    model.objects[0].scale.setZ(z);
}

void OnexNStgData::setTexture(int index, int texture) {
    model.groups[index].texture = texture;
}

OnexNStgData::~OnexNStgData() {
}
