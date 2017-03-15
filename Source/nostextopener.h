#ifndef NOSTEXTOPENER_H
#define NOSTEXTOPENER_H

#include <QObject>
#include "inosfileopener.h"

class NosTextOpener : public QObject, INosFileOpener
{
    Q_OBJECT
private:

public:
    NosTextOpener();
    virtual bool decrypt(QFile& file);
};

#endif // NOSTEXTOPENER_H
