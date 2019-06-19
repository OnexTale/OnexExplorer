#include "INosFileOpener.h"

int INosFileOpener::readNextInt(QFile &file) {
    return qFromLittleEndian<int>(reinterpret_cast<const uchar *>(file.read(4).data()));
}

QByteArray INosFileOpener::writeNextInt(int number) {
    QByteArray writeArray;
    writeArray.resize(4);
    qToLittleEndian<int>(number, reinterpret_cast<uchar *>(writeArray.data()));
    return writeArray;
}

QString INosFileOpener::neatFileName(QString fileName) {
    QStringList list = fileName.split("/");

    if (list.empty())
        return fileName;

    return list.back();
}
