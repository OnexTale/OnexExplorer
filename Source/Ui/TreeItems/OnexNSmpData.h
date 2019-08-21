#ifndef ONEXNSMPDATA_H
#define ONEXNSMPDATA_H

#include "../../Openers/NosZlibOpener.h"
#include "OnexTreeZlibItem.h"

class OnexNSmpData : public OnexTreeZlibItem {
Q_OBJECT
public:
    OnexNSmpData(const QString &name, QByteArray content, NosZlibOpener *opener,
                 int id, int creationDate, bool compressed);
    ~OnexNSmpData() override;
    QWidget *getPreview() override;
    QByteArray getContent() override;
    OnexTreeItem *addFrame(QByteArray content = QByteArray(), short width = 0, short height = 0, short xOrigin = 0, short yOrigin = 0);
public slots:
    void setName(QString name) override;
    int afterReplace(QByteArray content) override;
signals:
    void replaceSignal(QList<QImage> *newImages);
};

#endif // ONEXNSMPDATA_H
