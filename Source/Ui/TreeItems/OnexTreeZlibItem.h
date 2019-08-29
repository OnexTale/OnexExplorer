#ifndef ONEXTREEZLIBITEM_H
#define ONEXTREEZLIBITEM_H

#include "../../Openers/NosZlibOpener.h"
#include "../OnexTreeItem.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

class OnexTreeZlibItem : public OnexTreeItem {
Q_OBJECT
public:
    OnexTreeZlibItem(const QString &name, QByteArray content, NosZlibOpener *opener, int id,
                     int creationDate, bool compressed);
    ~OnexTreeZlibItem() override;
    int getId();
    int getCreationDate();
    bool isCompressed();
public slots:
    void setName(QString name) override;
    virtual void setId(int id, bool update = false);
    void setCreationDate(const QString &date, bool update = false);
    void setCompressed(bool compressed, bool update = false);
protected:
    int id;
    int creationDate;
    bool compressed;
    FileInfo *generateInfos() override;
    QString getDateAsString();
};

#endif // ONEXTREEZLIBITEM_H
