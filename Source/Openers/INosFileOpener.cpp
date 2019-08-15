#include "INosFileOpener.h"

QString INosFileOpener::neatFileName(QString fileName) {
    QStringList list = fileName.split("/");
    if (list.empty())
        return fileName;
    return list.back();
}

LittleEndianConverter *INosFileOpener::getLittleEndianConverter() {
    return &littleEndianConverter;
}