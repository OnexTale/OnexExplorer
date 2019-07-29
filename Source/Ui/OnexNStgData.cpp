#include "OnexNStgData.h"
#include "SingleModelPreview.h"

OnexNStgData::OnexNStgData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeZlibItem(header, name, content, opener, id, creationDate, compressed) {
    this->modelConverter = new NosModelConverter(content);
}

QWidget *OnexNStgData::getPreview() {
    if (!hasParent())
        return nullptr;
    return new SingleModelPreview(modelConverter->getVertices(), modelConverter->getFaces(), modelConverter->getNormals());
}

QWidget *OnexNStgData::getInfos() {
    if (!hasParent())
        return nullptr;

    QWidget *w = OnexTreeZlibItem::getInfos();
    QGridLayout *infoLayout = static_cast<QGridLayout *>(w->layout());

    QLabel *xLabel = new QLabel("x-Position");
    infoLayout->addWidget(xLabel, 7, 0);
    QLineEdit *xIn = new QLineEdit(QString::number(modelConverter->getPosition(0).x()));
    connect(xIn, SIGNAL(textChanged(QString)), this, SLOT(setXPosition(QString)));
    infoLayout->addWidget(xIn, 7, 1);

    QLabel *yLabel = new QLabel("y-Position");
    infoLayout->addWidget(yLabel, 8, 0);
    QLineEdit *yIn = new QLineEdit(QString::number(modelConverter->getPosition(0).y()));
    connect(yIn, SIGNAL(textChanged(QString)), this, SLOT(setYPosition(QString)));
    infoLayout->addWidget(yIn, 8, 1);

    QLabel *zLabel = new QLabel("z-Position");
    infoLayout->addWidget(zLabel, 9, 0);
    QLineEdit *zIn = new QLineEdit(QString::number(modelConverter->getPosition(0).z()));
    connect(zIn, SIGNAL(textChanged(QString)), this, SLOT(setZPosition(QString)));
    infoLayout->addWidget(zIn, 9, 1);

    QLabel *xRotationLabel = new QLabel("x-Rotation");
    infoLayout->addWidget(xRotationLabel, 10, 0);
    QLineEdit *xRotationIn = new QLineEdit(QString::number(modelConverter->getRotation(0).x()));
    connect(xRotationIn, SIGNAL(textChanged(QString)), this, SLOT(setXRotation(QString)));
    infoLayout->addWidget(xRotationIn, 10, 1);

    QLabel *yRotationLabel = new QLabel("y-Rotation");
    infoLayout->addWidget(yRotationLabel, 11, 0);
    QLineEdit *yRotationIn = new QLineEdit(QString::number(modelConverter->getRotation(0).y()));
    connect(yRotationIn, SIGNAL(textChanged(QString)), this, SLOT(setYRotation(QString)));
    infoLayout->addWidget(yRotationIn, 11, 1);

    QLabel *zRotationLabel = new QLabel("z-Rotation");
    infoLayout->addWidget(zRotationLabel, 12, 0);
    QLineEdit *zRotationIn = new QLineEdit(QString::number(modelConverter->getRotation(0).z()));
    connect(zRotationIn, SIGNAL(textChanged(QString)), this, SLOT(setZRotation(QString)));
    infoLayout->addWidget(zRotationIn, 12, 1);

    QLabel *wRotationLabel = new QLabel("w-Rotation");
    infoLayout->addWidget(wRotationLabel, 13, 0);
    QLineEdit *wRotationIn = new QLineEdit(QString::number(modelConverter->getRotation(0).w()));
    connect(wRotationIn, SIGNAL(textChanged(QString)), this, SLOT(setWRotation(QString)));
    infoLayout->addWidget(wRotationIn, 13, 1);

    QLabel *xScaleLabel = new QLabel("x-Scale");
    infoLayout->addWidget(xScaleLabel, 14, 0);
    QLineEdit *xScaleIn = new QLineEdit(QString::number(modelConverter->getScale(0).x()));
    connect(xScaleIn, SIGNAL(textChanged(QString)), this, SLOT(setXScale(QString)));
    infoLayout->addWidget(xScaleIn, 14, 1);

    QLabel *yScaleLabel = new QLabel("y-Scale");
    infoLayout->addWidget(yScaleLabel, 15, 0);
    QLineEdit *yScaleIn = new QLineEdit(QString::number(modelConverter->getScale(0).y()));
    connect(yScaleIn, SIGNAL(textChanged(QString)), this, SLOT(setYScale(QString)));
    infoLayout->addWidget(yScaleIn, 15, 1);

    QLabel *zScaleLabel = new QLabel("z-Scale");
    infoLayout->addWidget(zScaleLabel, 16, 0);
    QLineEdit *zScaleIn = new QLineEdit(QString::number(modelConverter->getScale(0).z()));
    connect(zScaleIn, SIGNAL(textChanged(QString)), this, SLOT(setZScale(QString)));
    infoLayout->addWidget(zScaleIn, 16, 1);

    return w;
}

int OnexNStgData::onExport(QString directory) {
    QFile file(directory + name + ".obj");
    file.open(QIODevice::WriteOnly);
    file.write(this->modelConverter->toObjFile(this->name).toLocal8Bit());
    file.close();

    QFile file2(directory + name + ".mtl");
    file2.open(QIODevice::WriteOnly);
    file2.write(this->modelConverter->toMtlFile().toLocal8Bit());
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

    ObjConverter *oc = new ObjConverter(obj);
    QByteArray newContent = content.mid(0, 0x30);
    newContent.append(oc->getNosFormat(0.00005));

    content = newContent;
    return 1;
}

void OnexNStgData::setXPosition(float x) {
    int offset = this->modelConverter->getObjectInfoOffset() + 2;
    content.replace(offset, 4, fromFloatToLittleEndian(x));
}

void OnexNStgData::setYPosition(float y) {
    int offset = this->modelConverter->getObjectInfoOffset() + 6;
    content.replace(offset, 4, fromFloatToLittleEndian(y));
}

void OnexNStgData::setZPosition(float z) {
    int offset = this->modelConverter->getObjectInfoOffset() + 10;
    content.replace(offset, 4, fromFloatToLittleEndian(z));    
}

void OnexNStgData::setXRotation(float x) {
    int offset = this->modelConverter->getObjectInfoOffset() + 14;
    content.replace(offset, 2, fromShortToLittleEndian(x * 0x7FFF));    
}
void OnexNStgData::setYRotation(float y) {
    int offset = this->modelConverter->getObjectInfoOffset() + 16;
    content.replace(offset, 2, fromShortToLittleEndian(y * 0x7FFF));    
}
void OnexNStgData::setZRotation(float z) {
    int offset = this->modelConverter->getObjectInfoOffset() + 18;
    content.replace(offset, 2, fromShortToLittleEndian(z * 0x7FFF));    
}
void OnexNStgData::setWRotation(float w) {
    int offset = this->modelConverter->getObjectInfoOffset() + 20;
    content.replace(offset, 2, fromShortToLittleEndian(w * 0x7FFF));    
}

void OnexNStgData::setXScale(float x) {
    int offset = this->modelConverter->getObjectInfoOffset() + 22;
    content.replace(offset, 4, fromFloatToLittleEndian(x));
}

void OnexNStgData::setYScale(float y) {
    int offset = this->modelConverter->getObjectInfoOffset() + 26;
    content.replace(offset, 4, fromFloatToLittleEndian(y));
}

void OnexNStgData::setZScale(float z) {
    int offset = this->modelConverter->getObjectInfoOffset() + 28;
    content.replace(offset, 4, fromFloatToLittleEndian(z));    
}

void OnexNStgData::setXPosition(QString x) {
    setXPosition(x.toFloat());
}

void OnexNStgData::setYPosition(QString y) {
    setYPosition(y.toFloat());
}
void OnexNStgData::setZPosition(QString z) {
    setZPosition(z.toFloat());
}

void OnexNStgData::setXRotation(QString x) {
    setXRotation(x.toFloat());
}

void OnexNStgData::setYRotation(QString y) {
    setYRotation(y.toFloat());
}

void OnexNStgData::setZRotation(QString z) {
    setZRotation(z.toFloat());
}

void OnexNStgData::setWRotation(QString w) {
    setWRotation(w.toFloat());
}

void OnexNStgData::setXScale(QString x) {
    setXScale(x.toFloat());
}

void OnexNStgData::setYScale(QString y) {
    setYScale(y.toFloat());
}
void OnexNStgData::setZScale(QString z) {
    setZScale(z.toFloat());
}

OnexNStgData::~OnexNStgData() {
}
