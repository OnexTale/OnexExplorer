#ifndef INOSFILEOPENER_H
#define INOSFILEOPENER_H

#include <QFile>
#include <QtEndian>

class OnexTreeItem;

class INosFileOpener {
public:
    virtual OnexTreeItem *decrypt(QFile &file) = 0;
    virtual QByteArray encrypt(OnexTreeItem *item) = 0;
protected:
    int readNextInt(QFile &file);
    QByteArray writeNextInt(int number);
    QString neatFileName(QString fileName);
};

#endif // INOSFILEOPENER_H
