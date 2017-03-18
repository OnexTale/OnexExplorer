#ifndef INOSFILEOPENER_H
#define INOSFILEOPENER_H
#include <QFile>

class INosFileOpener
{
public:
    virtual bool decrypt(QFile& file) = 0;
};

#endif // INOSFILEOPENER_H
