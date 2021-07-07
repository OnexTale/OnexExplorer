#ifndef NOSCCINFOPENER_H
#define NOSCCINFOPENER_H

#include <QObject>
#include <QDebug>
#include "INosFileOpener.h"

class NosCCInfOpener : public QObject, INosFileOpener
{
    Q_OBJECT
public:
    NosCCInfOpener();
    virtual OnexTreeItem* decrypt(QFile& file);
    virtual QByteArray encrypt(OnexTreeItem* item);

};

#endif // NOSCCINFOPENER_H
