#include "OnexNStgData.h"
#include "SingleModelPreview.h"

OnexNStgData::OnexNStgData(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                           bool compressed)
    : OnexTreeZlibItem(name, content, opener, id, creationDate, compressed) {
    this->modelConverter = new NosModelConverter(content);
}

QWidget *OnexNStgData::onClicked() {
    if (!hasParent())
        return nullptr;
    return    new SingleModelPreview(modelConverter->getVertices(), modelConverter->getFaces(), modelConverter->getNormals());
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

OnexNStgData::~OnexNStgData() {
}
