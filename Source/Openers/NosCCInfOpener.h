#ifndef NOSCCINFOPENER_H
#define NOSCCINFOPENER_H

#include "INosFileOpener.h"
#include <QDebug>
#include <QObject>

class NosCCInfOpener : public QObject, public INosFileOpener {
    Q_OBJECT
    public:
    NosCCInfOpener();
    virtual OnexTreeItem *decrypt(QFile &file);
    virtual QByteArray encrypt(OnexTreeItem *item);
};

#endif // NOSCCINFOPENER_H
