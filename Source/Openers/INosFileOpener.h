#ifndef INOSFILEOPENER_H
#define INOSFILEOPENER_H
#include <QFile>
#include <QtEndian>

class INosFileOpener
{
protected:
    int readNextInt(QFile& file);
public:
    virtual bool decrypt(QFile& file) = 0;
};

#endif // INOSFILEOPENER_H
