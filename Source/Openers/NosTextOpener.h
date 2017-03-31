#ifndef NOSTEXTOPENER_H
#define NOSTEXTOPENER_H

#include <QObject>
#include <QDebug>
#include "INosFileOpener.h"
#include "../Decryptors/NosTextDatFileDecryptor.h"
#include "../NosEnumTypes.h"

class NosTextOpener : public QObject, INosFileOpener
{
    Q_OBJECT
private:
    NosTextDatFileDecryptor datDecryptor;
public:
    NosTextOpener();
    virtual OnexTreeItem* decrypt(QFile& file);

};

#endif // NOSTEXTOPENER_H
