#ifndef INOSFILEOPENER_H
#define INOSFILEOPENER_H

#include <QFile>
#include <QtEndian>
#include "../Converters/LittleEndianConverter.h"

class OnexTreeItem;

class INosFileOpener {
public:
    virtual OnexTreeItem *decrypt(QFile &file) = 0;
    virtual QByteArray encrypt(OnexTreeItem *item) = 0;
    LittleEndianConverter *getLittleEndianConverter();
    virtual OnexTreeItem *getEmptyItem(const QByteArray &header) = 0;
protected:
    LittleEndianConverter littleEndianConverter;
    QString neatFileName(QString fileName);
};

#endif // INOSFILEOPENER_H
