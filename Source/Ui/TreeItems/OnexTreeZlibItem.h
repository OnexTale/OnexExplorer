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
    OnexTreeZlibItem(QByteArray header, const QString &name, QByteArray content, NosZlibOpener *opener, int id,
                     int creationDate, bool compressed);
    ~OnexTreeZlibItem() override;
    QWidget *getPreview() override;
    QByteArray getHeader();
    int getId();
    int getCreationDate();
    bool isCompressed();
public slots:
    void setHeader(const QString &header, bool update = false);
    void setId(int id, bool update = false);
    void setCreationDate(const QString &date, bool update = false);
    void setCompressed(bool compressed, bool update = false);
protected:
    int id;
    int creationDate;
    bool compressed;
    QByteArray header;
    FileInfo *generateInfos() override;
    QString getDateAsString();
};

#endif // ONEXTREEZLIBITEM_H
