#ifndef NOSTEXTOPENER_H
#define NOSTEXTOPENER_H

#include <QObject>
#include <QDebug>
#include "INosFileOpener.h"
#include "../Decryptors/NosTextDatFileDecryptor.h"
#include "../Ui/OnexTreeText.h"

class NosTextOpener : public QObject, INosFileOpener
{
    Q_OBJECT
private:
    NosTextDatFileDecryptor datDecryptor;
public:
    NosTextOpener();
    virtual OnexTreeItem* decrypt(QFile& file);
    virtual QByteArray encrypt(OnexTreeItem* item);

};

#endif // NOSTEXTOPENER_H
