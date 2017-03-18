#ifndef NOSTEXTOPENER_H
#define NOSTEXTOPENER_H

#include <QObject>
#include <QDebug>
#include "INosFileOpener.h"
#include "../Decryptors/NosTextDatFileDecryptor.h"

class NosTextOpener : public QObject, INosFileOpener
{
    Q_OBJECT
private:
    NosTextDatFileDecryptor datDecryptor;
public:
    NosTextOpener();
    virtual bool decrypt(QFile& file);

};

#endif // NOSTEXTOPENER_H
