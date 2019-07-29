#ifndef NOSTEXTOPENER_H
#define NOSTEXTOPENER_H

#include "../Decryptors/NosTextDatFileDecryptor.h"
#include "../Decryptors/NosTextOthersFileDecryptor.h"
#include "../Ui/OnexTreeText.h"
#include "INosFileOpener.h"
#include <QDebug>
#include <QObject>

class NosTextOpener : public QObject, public INosFileOpener {
    Q_OBJECT
    private:
    NosTextDatFileDecryptor datDecryptor;
    NosTextOthersFileDecryptor lstDecryptor;

    public:
    NosTextOpener();
    virtual OnexTreeItem *decrypt(QFile &file);
    virtual QByteArray encrypt(OnexTreeItem *item);
};

#endif // NOSTEXTOPENER_H
