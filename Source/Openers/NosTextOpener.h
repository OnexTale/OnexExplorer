#ifndef NOSTEXTOPENER_H
#define NOSTEXTOPENER_H

#include "../Decryptors/NosTextDatFileDecryptor.h"
#include "../Decryptors/NosTextOthersFileDecryptor.h"
#include "../Ui/TreeItems/OnexTreeText.h"
#include "INosFileOpener.h"
#include <QDebug>
#include <QObject>

class NosTextOpener : public QObject, public INosFileOpener {
Q_OBJECT
public:
    NosTextOpener();
    OnexTreeItem *decrypt(QFile &file);
    QByteArray encrypt(OnexTreeItem *item);
    OnexTreeItem *getEmptyItem(const QByteArray &header) override;
private:
    NosTextDatFileDecryptor *datDecryptor;
    NosTextOthersFileDecryptor *lstDecryptor;
    QByteArray generateOLETIME();
    QString readOLETIME(QByteArray array);
};

#endif // NOSTEXTOPENER_H
