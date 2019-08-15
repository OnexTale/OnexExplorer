#ifndef ONEXNSMPDATA_H
#define ONEXNSMPDATA_H

#include "../../Openers/NosZlibOpener.h"
#include "OnexTreeZlibItem.h"

class OnexNSmpData : public OnexTreeZlibItem {
Q_OBJECT
public:
    OnexNSmpData(const QByteArray &header, const QString &name, const QByteArray &content, NosZlibOpener *opener,
                 int id, int creationDate, bool compressed);
    ~OnexNSmpData() override;
    QWidget *getPreview() override;
    QByteArray getContent() override;
public slots:
    int afterReplace(QByteArray content) override;
signals:
    void replaceSignal(QList<QImage> *newImages);
};

#endif // ONEXNSMPDATA_H
