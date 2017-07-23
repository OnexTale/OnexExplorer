#ifndef INOSFILEOPENER_H
#define INOSFILEOPENER_H
#include <QFile>
#include <QtEndian>
#include "../Ui/OnexTreeItem.h"

class INosFileOpener
{
protected:
    int readNextInt(QFile& file);
    QByteArray writeNextInt(int number);
    QString neatFileName(QString fileName);
public:
    virtual OnexTreeItem* decrypt(QFile& file) = 0;
    virtual QByteArray encrypt(OnexTreeItem* item) = 0;
};

#endif // INOSFILEOPENER_H
