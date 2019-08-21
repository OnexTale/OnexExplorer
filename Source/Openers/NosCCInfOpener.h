#ifndef NOSCCINFOPENER_H
#define NOSCCINFOPENER_H

#include "INosFileOpener.h"
#include <QDebug>
#include <QObject>

class NosCCInfOpener : public QObject, public INosFileOpener {
Q_OBJECT
public:
    NosCCInfOpener();
    OnexTreeItem *decrypt(QFile &file);
    QByteArray encrypt(OnexTreeItem *item);
    OnexTreeItem *getEmptyItem(const QByteArray &header) override;
};

#endif // NOSCCINFOPENER_H
